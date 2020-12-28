#include <fstream>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include "packet.h"
#include "process_packet.h"
#include "capture.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;

uint64_t Capture::total_unparsed_packets = 0;
queue<packet> Capture::packet_queue;

Capture::Capture(string _interface, string _debugpath, string _packetpath, int _window, int _max_sizelog, int _max_files)
{
    if (_interface != "")
        Capture::interface = _interface;
    if (_debugpath != "")
        Capture::debugpath = _debugpath;
    if (_packetpath != "")
        Capture::packetpath = _packetpath;
    if (_window != 0)
        Capture::window = _window;
    if (_max_sizelog != 0)
        Capture::max_sizelog = _max_sizelog;
    if (_max_files != 0)
        Capture::window = _max_files;
}

/*
    start_pfring_capture():
        Choose an ethernet interface to capture, set sampling rate(?).
*/

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

    // Initialize PF_RING
    const char *device_name = interface.c_str();
    bool pfring_init_result = start_pfring_packet_preprocessing(device_name);
    if (!pfring_init_result)
    {
        // Internal error in PF_RING
        spdlog::get("exec_logger")->error("PF_RING initilization failed, exit from program");
        exit(1);
    }
}

/*
    parsing_pfring_packet():
        Parsing PF_RING packet.
*/

void Capture::parsing_pfring_packet()
{

    // A packet. Description of all fields: "packet.h"
    packet current_packet;
    memset((void *)&header->extended_hdr.parsed_pkt, 0, sizeof(header->extended_hdr.parsed_pkt));

    // Capture packet
    u_int8_t timestamp = 0; // We do not calculate timestamps here because it's useless and consumes so much CPU
    u_int8_t add_hash = 0;  // https://github.com/ntop/PF_RING/issues/9
    pfring_parse_pkt((u_char *)buffer, (struct pfring_pkthdr *)header, 4, timestamp, add_hash);

    // Ignores non IPv4 and IPv6 packet
    if (header->extended_hdr.parsed_pkt.ip_version != 4 && header->extended_hdr.parsed_pkt.ip_version != 6)
    {
        Capture::total_unparsed_packets++;
        return;
    }

    // Get packet IP version to our psuedo header
    current_packet.ip_protocol_version = header->extended_hdr.parsed_pkt.ip_version;

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

    // We need this for deep packet inspection
    current_packet.packet_payload_length = header->len;
    current_packet.packet_payload_pointer = (void *)buffer;

    // Get other data to our psuedo header
    current_packet.length = header->len;
    current_packet.protocol = header->extended_hdr.parsed_pkt.l3_proto;
    current_packet.ts = header->ts;

    // Copy flags from PF_RING header to our pseudo header
    if (current_packet.protocol == IPPROTO_TCP)
    {
        current_packet.flags = header->extended_hdr.parsed_pkt.tcp.flags;
    }
    else
    {
        current_packet.flags = 0;
    }

    //log_packet_summary flow
    packet_queue.push(current_packet);
    if (current_packet.packetCounter % window == 0){
        process_packet(packet_queue, packet_logger);
    }
}

/*
start_pfring_packet_preprocessing():
    Intialize PF_RING variables and start capturing;
    Get info about PF_RING version;
    Set application name to 'fireflow';
    etc...
    [Args:] const char* dev: Name of the device we want to capture.
*/

bool Capture::start_pfring_packet_preprocessing(char *dev)
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
    ring = new PFring(dev, snaplen, flags);
    if (!ring)
    {
        spdlog::get("exec_logger")->error("pfring_open() error: {}",strerror(errno));
        spdlog::get("exec_logger")->error("pf_ring not loaded or perhaps you use quick mode and have already a socket bound to: {}",dev);
        return false;
    }
    // Print successful message!!
    spdlog::get("exec_logger")->info("Successully binded to: {}",dev);
    spdlog::get("exec_logger")->info("Device RX channels number: {}", int(ring->get_num_rx_channels()));

    // Set application name in /proc
    int pfring_appname_result = ring->set_application_name((char *)"fireflow");
    if (pfring_appname_result != 0)
        spdlog::get("exec_logger")->error("Can't set program name for PF_RING: pfring_set_application_name");
    // Get PF_RING version
    u_int32_t version;
    ring->get_version(&version);
    spdlog::get("exec_logger")->info("Using PF_RING v.{}.{}.{}",(version & 0xFFFF0000) >> 16, (version & 0x0000FF00) >> 8, (version & 0x000000FF));
    // Set socket mode to RECEIVE ONLY
    int pfring_socketmode_result = ring->set_socket_mode(recv_only_mode);
    if (pfring_socketmode_result != 0)
        spdlog::get("exec_logger")->info("pfring_set_socket_mode returned [rc={}]\n", pfring_socketmode_result);
    // Enable ring
    if (ring->enable_ring() != 0)
    {
        spdlog::get("exec_logger")->info("Unable to enable ring :-(");
        ring->close();
        return false;
    }
    // Set wait-for-packet mode & capture
    u_int8_t wait_for_packet = 1;
    const struct pfring_pkthdr *header;
    const u_char *buffer;
    const u_char *user_bytes;
    while (true){
        
    }
    return true;
};

/*
    stop_pfring_capture():  
        Shuts down PF_RING capture.
*/

void Capture::stop_pfring_capture()
{
    pfring_breakloop(ring);
}