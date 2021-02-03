#include <fstream>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "packet.h"
#include "entropy.h"
#include "capture.h"
#include "cusum.h"
#include "detector.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;
uint Capture::total_unparsed_packets = 0;
int Capture::estimator = 0;
double Capture::window = 0;
double Capture::subwindow = 0;
ulong packet::totalPackets = 0;

Capture::Capture(string _interface, string _debugpath, string _packetpath, double _window, double _subwindow, int _estimator, int _max_sizelog, int _max_files)
{
    Capture::interface = _interface;
    Capture::debugpath = _debugpath;
    Capture::packetpath = _packetpath;
    Capture::window = _window;
    Capture::subwindow = _subwindow;
    Capture::estimator = _estimator;
    Capture::max_sizelog = _max_sizelog;
    Capture::max_files = _max_files;
}

void Capture::start_pfring_capture()
{
    spdlog::get("exec_logger")->info("------Fireflow v1.0 execution logger----");
    spdlog::get("exec_logger")->info("PF_RING plugin started");
    spdlog::get("exec_logger")->info("We selected interface: " + interface);
    if (interface == "")
    {
        spdlog::get("exec_logger")->warn("[!] Please specify interface");
        exit(1);
    }

    const char *device_name = interface.c_str();
    bool pfring_init_result = start_pfring_packet_preprocessing(device_name); // Initialize PF_RING
    if (!pfring_init_result)
    {
        spdlog::get("exec_logger")->error("PF_RING initilization failed, exit from program"); // Internal error in PF_RING
        exit(1);
    }
}

bool Capture::start_pfring_packet_preprocessing(const char *dev)
{
    // Setting variables to set flags

    bool promisc = true;                 // Enable promiscuous mode
    bool use_extended_pkt_header = true; // PF_RING fills the field extended_hdr of struct pfring_pkthdr to get extra information
    bool enable_hw_timestamp = false;    // Get timestamp from hardware
    bool dont_strip_timestamps = false;  // Don't strip HW timestamp from the packet
    bool pfring_kernel_parser = true;    // Enable packet parsing

    // Set Flag for capture
    u_int32_t flags = 0;
    if (use_extended_pkt_header)
        flags |= PF_RING_LONG_HEADER;
    if (promisc)
        flags |= PF_RING_PROMISC;
    if (enable_hw_timestamp)
        flags |= PF_RING_HW_TIMESTAMP;
    if (!dont_strip_timestamps)
        flags |= PF_RING_STRIP_HW_TIMESTAMP;
    if (!pfring_kernel_parser)
        flags |= PF_RING_DO_NOT_PARSE;
    flags |= PF_RING_DNA_SYMMETRIC_RSS; /* Note that symmetric RSS is ignored by non-DNA drivers */
    unsigned int snaplen = 128;
    ring = pfring_open(dev, snaplen, flags);
    if (!ring)
    {
        spdlog::get("exec_logger")->error("pfring_open() error: {}", strerror(errno));
        spdlog::get("exec_logger")->error("pf_ring not loaded or perhaps you use quick mode and have already a socket bound to: {}", dev);
        return false;
    }

    // Print successful message!!
    spdlog::get("exec_logger")->info("Successully binded to: {}", dev);
    spdlog::get("exec_logger")->info("Device RX channels number: {}", int(pfring_get_num_rx_channels(ring)));

    // Set application name in /proc
    int pfring_appname_result = pfring_set_application_name(ring, (char *)"fireflow");
    if (pfring_appname_result != 0)
        spdlog::get("exec_logger")->error("Can't set program name for PF_RING: pfring_set_application_name");

    // Get PF_RING version

    u_int32_t version;
    pfring_version(ring, &version);
    spdlog::get("exec_logger")->info("Using PF_RING v.{}.{}.{}", (version & 0xFFFF0000) >> 16, (version & 0x0000FF00) >> 8, (version & 0x000000FF));

    // Set socket mode to RECEIVE ONLY

    int pfring_socketmode_result = pfring_set_socket_mode(ring, recv_only_mode);
    if (pfring_socketmode_result != 0)
        spdlog::get("exec_logger")->info("pfring_set_socket_mode returned [rc={}]\n", pfring_socketmode_result);

    // Enable ring
    if (pfring_enable_ring(ring) != 0)
    {
        spdlog::get("exec_logger")->info("Unable to enable ring :-(");
        pfring_close(ring);
        return false;
    }

    while (true)
    {
        int pfring_recv_int = 0;
        struct pfring_pkthdr hdr;
        u_char *buffer;
        u_int8_t wait_for_packet = 0;
        pfring_recv_int = pfring_recv(ring, &buffer, 0, &hdr, wait_for_packet);
        execution_flow(hdr, buffer, pfring_recv_int);
    }
    return true;
};

void Capture::execution_flow(const struct pfring_pkthdr &hdr, const u_char *buffer, int hasPkt)
{
    auto current_t = chrono::steady_clock::now();
    bool entropyIsCalced = false;
    double duration;
    double duration_window;
    double threshold_tracking;
    packet current_packet;
    static auto runtime = chrono::steady_clock::now();
    static auto subwindow_t = chrono::steady_clock::now();
    static auto window_t = chrono::steady_clock::now();
    static uint packet_queue = 0;
    static EntropyCalc EntropyTask;
    static Cusum CusumTask;
    static Detector DetectorTask;

    //track if the time for setting threshold has been reached yet
    if (CusumTask.getThresholdStatus() == false)
    {
        threshold_tracking = (chrono::duration_cast<chrono::microseconds>(current_t - runtime).count()) / 1000000.0;
    }

    // track if the duration for collecting packets in a subwindow has ended yet
    duration = (chrono::duration_cast<chrono::microseconds>(current_t - subwindow_t).count()) / 1000000.0;
    duration_window = (chrono::duration_cast<chrono::microseconds>(current_t - window_t).count()) / 1000000.0;
        entropyIsCalced = false;
    if (duration < Capture::subwindow)
    {
        if (hasPkt == 1)
        {
            packet_queue += 1;
            current_packet = parsing_pfring_packet(&hdr, buffer);
            EntropyTask.accumulate_packets(current_packet);
        };
    }

    // if the durat ion for collecting packets has ended, start calculating entropies of that duration
    if (duration >= Capture::subwindow)
    {
        if (packet_queue > 0)
        {
            current_packet = parsing_pfring_packet(&hdr, buffer);
            EntropyTask.accumulate_subwindow_entropies(current_packet);
        }
        else
        {
            // if no packets were received then 0 entropy
            EntropyTask.zero_entropy();
        }
        packet_queue = 0;
        entropyIsCalced = true;
        subwindow_t = chrono::steady_clock::now();
    }
    if (CusumTask.getThresholdStatus() == true && entropyIsCalced == true)
    {
        if (duration_window >= Capture::window)
        {
            CusumTask.calc(EntropyTask.getLatestEntropies());
            DetectorTask.judgeCusum(CusumTask);
            window_t = chrono::steady_clock::now();
        }
    }
    if (CusumTask.getThresholdStatus() == false && entropyIsCalced == true)
    {
        if (threshold_tracking > window * estimator)
        {
            CusumTask.setSampleSize(Capture::window / Capture::subwindow * Capture::estimator);
            CusumTask.setSubGroupSize(Capture::window / Capture::subwindow);
            CusumTask.setThreshold(EntropyTask.GetFullEntropies());
        }
    }
}

packet Capture::parsing_pfring_packet(const struct pfring_pkthdr *header, const u_char *buffer)
{
    // A packet. Description of all fields: "packet.h"
    packet current_packet;
    memset((void *)&header->extended_hdr.parsed_pkt, 0, sizeof(header->extended_hdr.parsed_pkt));

    // Capture packet
    u_int8_t timestamp = 0;
    u_int8_t add_hash = 0; // https://github.com/ntop/PF_RING/issues/9
    pfring_parse_pkt((u_char *)buffer, (struct pfring_pkthdr *)header, 4, timestamp, add_hash);

    // Ignores non IPv4 and IPv6 packet
    if (header->extended_hdr.parsed_pkt.ip_version != 4 && header->extended_hdr.parsed_pkt.ip_version != 6)
    {
        Capture::total_unparsed_packets++;
    }

    // Get packet IP version to our psuedo header
    current_packet.ip_protocol_version = header->extended_hdr.parsed_pkt.ip_version;
    //current_packet.ts.tv_nsec = header->extended_hdr.timestamp_ns;
    // Get IPv4 source/dest to our psuedo header
    if (current_packet.ip_protocol_version == 4)
    {
        // PF_RING stores data in host byte order but we use network byte order
        current_packet.src_ip = htonl(header->extended_hdr.parsed_pkt.ip_src.v4);
        current_packet.dst_ip = htonl(header->extended_hdr.parsed_pkt.ip_dst.v4);
    }

    // Get port to our psuedo header
    current_packet.src_port = header->extended_hdr.parsed_pkt.l4_src_port;
    current_packet.dst_port = header->extended_hdr.parsed_pkt.l4_dst_port;
    current_packet.ts = header->ts;
    // We need this for deep packet inspection
    current_packet.packet_payload_length = header->len;
    current_packet.packet_payload_pointer = (void *)buffer;

    // Get other data to our psuedo header
    current_packet.length = header->len;
    current_packet.protocol = header->extended_hdr.parsed_pkt.l3_proto;

    // Copy flags from PF_RING header to our pseudo header
    if (current_packet.protocol == IPPROTO_TCP)
    {
        current_packet.flags = header->extended_hdr.parsed_pkt.tcp.flags;
    }
    else
    {
        current_packet.flags = 0;
    }
    return current_packet;
}

// stop_pfring_capture(): Shuts down PF_RING capture.
void Capture::stop_pfring_capture()
{
    pfring_breakloop(ring);
}