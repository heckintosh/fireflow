#include <log4cpp/Appender.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <fstream>
#include <iostream>
#include <string>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pfring.h>
#include "packet.h"
#include "process_packet.h"
#include "capture.h"

using namespace std;

string Capture::logfile_path = "/tmp/fireflow/pfring_init.log"; // Path to log file
string Capture::interface = "eth0";                        // The ethernet interface to capture packet
string Capture::packet_file = "/tmp/packetlogger.txt";    // The file contains packet's content
string *Capture::packet_file_ptr = &Capture::packet_file;

uint32_t Capture::pfring_sampling_rate = 100;
uint64_t Capture::total_unparsed_packets = 0;


Capture::Capture(string user_iface, string user_ringlog, string user_packlog)
{
    if (user_iface   != "") Capture::interface   = user_iface;
    if (user_ringlog != "") Capture::packet_file = user_ringlog; 
    if (user_packlog != "") Capture::packet_file = user_packlog; 

}

void Capture::init_logging()
{

    // Create pattern variable for logging: <date> [<priority>] <message>\n
    log4cpp::PatternLayout *layout = new log4cpp::PatternLayout();
    layout->setConversionPattern("%d [%p] %m%n");

    // Set output of the log file, with the pattern variable set
    log4cpp::Appender *appender = new log4cpp::FileAppender("default", logfile_path);
    appender->setLayout(layout);

    // Set priority to log the file and the appender to log into
    logger.setPriority(log4cpp::Priority::INFO);
    logger.addAppender(appender);

    // Log to the logger to indicate the logger is finished!
    logger.info("Logger initialized!");
}

/*
    start_pfring_capture():
        Choose an ethernet interface to capture, set sampling rate(?).
*/

void Capture::start_pfring_capture()
{

    // Check if interface is selected
    logger << log4cpp::Priority::INFO << "PF_RING plugin started";
    logger << log4cpp::Priority::INFO << "We selected interface: " << interface;
    if (interface == "")
    {
        logger << log4cpp::Priority::ERROR << "Please specify interface";
        exit(1);
    }

    // Initialize PF_RING
    const char *device_name = interface.c_str();
    bool pfring_init_result = start_pfring_packet_preprocessing(device_name);
    if (!pfring_init_result)
    {
        // Internal error in PF_RING
        logger << log4cpp::Priority::ERROR << "PF_RING initilization failed, exit from program";
        exit(1);
    }
}

/*
    parsing_pfring_packet():
        Parsing PF_RING packet.
*/

void Capture::parsing_pfring_packet(const struct pfring_pkthdr *header, const u_char *buffer, const u_char *user_bytes)
{

    // A packet. Description of all fields: "packet.h"
    packet current_packet;

    // Now we support only non sampled input from PF_RING
    current_packet.sample_ratio = Capture::pfring_sampling_rate;
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
    process_packet(current_packet, *(Capture::packet_file_ptr));
}

/*
start_pfring_packet_preprocessing():
    Intialize PF_RING variables and start capturing;
    Get info about PF_RING version;
    Set application name to 'fireflow';
    etc...
    [Args:] const char* dev: Name of the device we want to capture.
*/
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
    // Set length of data in each frame default as "pfcount.c"
    unsigned int snaplen = 128;
    ring = pfring_open(dev, snaplen, flags);
    if (!ring)
    {
        logger
            << log4cpp::Priority::INFO << "pfring_open() error: " << strerror(errno)
            << " (pf_ring not loaded or perhaps you use quick mode and have already a socket bound to: " << dev << ")";
        return false;
    }
    // Print successful message!!
    logger << log4cpp::Priority::INFO << "Successully binded to: " << dev;
    logger << log4cpp::Priority::INFO << "Device RX channels number: " << int(pfring_get_num_rx_channels(ring));
    // Set application name in /proc
    int pfring_appname_result = pfring_set_application_name(ring, (char *)"fireflow");
    if (pfring_appname_result != 0)
        logger << log4cpp::Priority::ERROR << "Can't set program name for PF_RING: pfring_set_application_name";
    // Get PF_RING version
    u_int32_t version;
    pfring_version(ring, &version);
    logger.info("Using PF_RING v.%d.%d.%d",
                (version & 0xFFFF0000) >> 16,
                (version & 0x0000FF00) >> 8,
                version & 0x000000FF);
    // Set socket mode to RECEIVE ONLY
    int pfring_socketmode_result = pfring_set_socket_mode(ring, recv_only_mode);
    if (pfring_socketmode_result != 0)
        logger.info("pfring_set_socket_mode returned [rc=%d]\n", pfring_socketmode_result);
    // Enable ring
    if (pfring_enable_ring(ring) != 0)
    {
        logger << log4cpp::Priority::INFO << "Unable to enable ring :-(";
        pfring_close(ring);
        return false;
    }
    // Set wait-for-packet mode & capture
    u_int8_t wait_for_packet = 1;
    pfring_loop(ring, Capture::parsing_pfring_packet,(u_char *)NULL, wait_for_packet);
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




