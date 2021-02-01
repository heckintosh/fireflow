#ifndef PACKET_HEADER
#define PACKET_HEADER
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>

// simplified packet struct for lightweight save into memory
class packet
{
public:
  static ulong totalPackets;
  packet()
      : sample_ratio(1), src_ip(0), dst_ip(0), src_port(0), dst_port(0), protocol(0),
        length(0), flags(0), ip_fragmented(false), ip_protocol_version(4),
        ttl(0), packet_payload_pointer(NULL), packet_payload_length(0)
  {
    totalPackets++;
    packetCounter = totalPackets;
  }

  ulong packetCounter;
  uint sample_ratio;
  /* IPv4 */
  uint src_ip;
  uint dst_ip;
  uint ip_protocol_version; /* IPv4 or IPv6 */
  uint ttl;
  uint src_port;
  uint dst_port;
  uint protocol;

  uint length;
  uint flags; /* tcp flags */

  bool ip_fragmented; /* If IP packet fragmented */

  struct timeval ts;
  void *packet_payload_pointer;
  uint packet_payload_length;
  uint packet_payload_full_length = 0; // In case of cropped packets we use this
};

#endif