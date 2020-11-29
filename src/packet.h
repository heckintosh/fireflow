#ifndef PACKET_HEADER
#define PACKET_HEADER
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

// simplified packet struct for lightweight save into memory
class packet {
    public:
    static int packetCounter;
    packet()
    : sample_ratio(1), src_ip(0), dst_ip(0), src_port(0), dst_port(0), protocol(0),
      length(0), flags(0), ip_fragmented(false), ip_protocol_version(4),
      ttl(0), packet_payload_pointer(NULL), packet_payload_length(0){packetCounter++;}

    uint32_t sample_ratio;
    /* IPv4 */
    uint32_t src_ip;
    uint32_t dst_ip;

    uint32_t input_interface  = 0;
    uint32_t output_interface = 0;

    uint8_t ip_protocol_version; /* IPv4 or IPv6 */
    uint8_t ttl;
    uint16_t src_port;
    uint16_t dst_port;
    unsigned int protocol;
    
    uint64_t length;
    uint8_t flags; /* tcp flags */

    bool ip_fragmented; /* If IP packet fragmented */

    struct timeval ts;
    void* packet_payload_pointer;
    uint32_t packet_payload_length;
    uint32_t packet_payload_full_length = 0; // In case of cropped packets we use this


    // Device uptime when flow started
    int64_t flow_start = 0;
    // Device uptime when flow fnishes
    int64_t flow_end = 0;
};

typedef void (*packet_process_ptr)(packet&);
#endif