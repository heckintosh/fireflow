#ifndef PACKET_HEADER
#define PACKET_HEADER
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
using namespace std;

// simplified packet struct for lightweight save into memory
class packet_t
{
private:
  static int number_of_packets;

public:
  uint64_t length, flow_start, flow_end;
  uint32_t sample_ratio, src_ip, dst_ip, input_interface, output_interface, packet_payload_length, packet_payload_full_length;
  uint8_t ip_protocol_version, ttl, flags;
  uint16_t source_port, destination_port;
  struct timeval ts;
  bool ip_fragmented;
  unsigned int protocol;
  void *packet_payload_pointer;

  packet_t(){
    sample_ratio = 1;
    src_ip = 0; 
    dst_ip = 0; 
    input_interface = 0;
    output_interface = 0;
    source_port = 0; 
    destination_port = 0;
    protocol = 0;
    length = 0;
    flags = 0; 
    ip_fragmented = false;
    ip_protocol_version = 4;
    ttl = 0; 
    packet_payload_pointer = NULL;
    packet_payload_length = 0;
    packet_payload_full_length = 0;
    flow_start = 0;
    flow_end = 0;
    number_of_packets++;
  }
};

typedef void (*packet_process_ptr)(packet_t &);
#endif