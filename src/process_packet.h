#ifndef PROCESS_PACKET_HEADER

#define PROCESS_PACKET_HEADER
#include <queue>
#include "packet.h"

using namespace std;

void process_packet(queue<packet> &packet_queue, ofstream& packetlog, int window_counter);
void countSourceIP_map(map<uint32_t, int> &src_ip_map, uint32_t src_ip);
void countDestIP_map(map<uint32_t, int> &dst_ip_map, uint32_t dst_ip);
void countSourcePort(map<uint16_t, int> &src_port_map, uint32_t src_port);
void countProtocol(map<uint8_t, int> &protocols_map, uint8_t protocol);
void countFlags(map<uint8_t, int> &flags_map, uint8_t flag);
void countDestPort(map<uint16_t, int> &dst_port_map, uint32_t dst_port);
string preDetectionInfo(queue <packet> &packet_queue, ofstream &packet_file,int window_counter);
string get_flags(uint8_t flag);
string log_packet(packet packet, ofstream &packet_file);
string get_protocol(uint8_t protocol);
string ip_int_to_string(uint32_t ip_as_integer);

#endif
