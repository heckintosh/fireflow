#include "log4cpp/Appender.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/Priority.hh"
#include "log4cpp/PatternLayout.hh"
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
log4cpp::Category& logger = log4cpp::Category::getRoot();

uint64_t  total_unparsed_packets;

#ifndef FIREFLOW
#include "CLI11.hpp"

string interface = "";
string logfile_path = "/tmp/fireflow/pfring_init.log";
string packet_file = "/tmp/packetlogger.txt";

int main(int argc, char* argv[]){
    CLI::App app{"FIREFLOW: USTH ANTI_DDOS"};

    std::string filename ;
    app.add_option("-i,--interface", interface, "Capture interface")->required();
    app.add_option("--log", logfile_path, "Dump to log file");
    
    CLI11_PARSE(app, argc, argv);
    
    init_logging();
    start_pfring_capture();
    
}   
#else
extern string interface;
extern string logfile_path;
extern string packet_file;

#endif


string *packet_file_ptr = &packet_file;
bool pfring_kernel_parser = true;
pfring* ring = NULL;
uint32_t pfring_sampling_rate = 1;

int packet::internalPacketCounter; // Why this is here ???


//Function declaration

void init_logging() {
    log4cpp::PatternLayout* layout = new log4cpp::PatternLayout();
    layout->setConversionPattern("%d [%p] %m%n");

    log4cpp::Appender* appender = new log4cpp::FileAppender("default", logfile_path);
    appender->setLayout(layout);

    logger.setPriority(log4cpp::Priority::INFO);
    logger.addAppender(appender);
    logger.info("Logger initialized!");
}

// Choose an ethernet interface to capture, set sampling rate
void start_pfring_capture() {
    logger << log4cpp::Priority::INFO << "PF_RING plugin started";
    //pfring_packetprocessor_ptr = func_ptr;
    logger << log4cpp::Priority::INFO << "We selected interface:" << interface;
    if (interface == "") {
        logger << log4cpp::Priority::ERROR << "Please specify interface";
        exit(1);
    }
    const char* device_name = interface.c_str();
    bool pfring_init_result = false;
    pfring_init_result = start_pfring_packet_preprocessing(device_name);
    if (!pfring_init_result) {
        // Internal error in PF_RING
        logger << log4cpp::Priority::ERROR << "PF_RING initilization failed, exit from program";
        exit(1);
    }
}    

bool start_pfring_packet_preprocessing(const char* dev) {

    bool promisc = true;
    bool use_extended_pkt_header = true;
    bool enable_hw_timestamp = false;
    bool dont_strip_timestamps = false;

    u_int32_t flags = 0;
    if (use_extended_pkt_header) flags |= PF_RING_LONG_HEADER;
    if (promisc) flags |= PF_RING_PROMISC;
    if (enable_hw_timestamp) flags |= PF_RING_HW_TIMESTAMP;
    if (!dont_strip_timestamps) flags |= PF_RING_STRIP_HW_TIMESTAMP;

    if (!pfring_kernel_parser) {
        flags |= PF_RING_DO_NOT_PARSE;
    }

    flags |= PF_RING_DNA_SYMMETRIC_RSS; /* Note that symmetric RSS is ignored by non-DNA drivers */

    // use default value from pfcount.c
    unsigned int snaplen = 128;

    ring = pfring_open(dev, snaplen, flags);

    if (ring == NULL) {
        logger
        << log4cpp::Priority::INFO << "pfring_open error: " << strerror(errno)
        << " (pf_ring not loaded or perhaps you use quick mode and have already a socket bound to: " << dev
        << ")";
        return false;
    }

    logger << log4cpp::Priority::INFO << "Successully binded to: " << dev;

    logger << log4cpp::Priority::INFO
           << "Device RX channels number: " << int(pfring_get_num_rx_channels(ring));

    u_int32_t version;
    // Set spplication name in /proc
    int pfring_appname_result = pfring_set_application_name(ring, (char*)"fireflow");

    if (pfring_appname_result != 0) {
        logger << log4cpp::Priority::ERROR << "Can't set program name for PF_RING: pfring_set_application_name";
    }

    pfring_version(ring, &version);

    logger.info("Using PF_RING v.%d.%d.%d", (version & 0xFFFF0000) >> 16,
                (version & 0x0000FF00) >> 8, version & 0x000000FF);

    int pfring_socketmode_result = pfring_set_socket_mode(ring, recv_only_mode);

    if (pfring_socketmode_result != 0) {
        logger.info("pfring_set_socket_mode returned [rc=%d]\n", pfring_socketmode_result);
    }

    // enable ring
    if (pfring_enable_ring(ring) != 0) {
        logger << log4cpp::Priority::INFO << "Unable to enable ring :-(";
        pfring_close(ring);
        return false;
    }

    u_int8_t wait_for_packet = 1;

    pfring_loop(ring, parsing_pfring_packet, (u_char*)NULL, wait_for_packet);

    return true;
}

void parsing_pfring_packet(const struct pfring_pkthdr* h, const u_char* p, const u_char* user_bytes) {
    // Description of all fields: http://www.ntop.org/pfring_api/structpkt__parsing__info.html
    packet current_packet;

    // Now we support only non sampled input from PF_RING
    current_packet.sample_ratio = pfring_sampling_rate;
    memset((void*)&h->extended_hdr.parsed_pkt, 0, sizeof(h->extended_hdr.parsed_pkt));

    // We do not calculate timestamps here because it's useless and consumes so much cpu
    // https://github.com/ntop/PF_RING/issues/9
    u_int8_t timestamp = 0;
    u_int8_t add_hash = 0;
    // pfring function
    pfring_parse_pkt((u_char*)p, (struct pfring_pkthdr*)h, 4, timestamp, add_hash);

    if (h->extended_hdr.parsed_pkt.ip_version != 4 && h->extended_hdr.parsed_pkt.ip_version != 6) {
        total_unparsed_packets++;
        return;
    }

    current_packet.ip_protocol_version = h->extended_hdr.parsed_pkt.ip_version;

    if (current_packet.ip_protocol_version == 4) {
        // IPv4

        /* PF_RING stores data in host byte order but we use network byte order */
        current_packet.src_ip = htonl(h->extended_hdr.parsed_pkt.ip_src.v4);
        current_packet.dst_ip = htonl(h->extended_hdr.parsed_pkt.ip_dst.v4);
    } 
    current_packet.src_port = h->extended_hdr.parsed_pkt.l4_src_port;
    current_packet.dst_port = h->extended_hdr.parsed_pkt.l4_dst_port;

    // We need this for deep packet inspection
    current_packet.packet_payload_length = h->len;
    current_packet.packet_payload_pointer = (void*)p; 

    current_packet.length = h->len;
    current_packet.protocol = h->extended_hdr.parsed_pkt.l3_proto;
    current_packet.ts = h->ts;

    // Copy flags from PF_RING header to our pseudo header
    if (current_packet.protocol == IPPROTO_TCP) {
        current_packet.flags = h->extended_hdr.parsed_pkt.tcp.flags;
    } else {
        current_packet.flags = 0;
    }
    process_packet(current_packet, *packet_file_ptr);
}

void stop_pfring_capture() {
    pfring_breakloop(ring);
}


