#ifndef PROCESS_PACKET_HEADER

#define PROCESS_PACKET_HEADER
#include <queue>
#include <map>
#include "packet.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;

void process_packet(queue<packet> &packet_queue, shared_ptr<spdlog::logger> &packetlog);
void mapSrcIP(map<uint32_t, int> &src_ip_map, uint32_t src_ip);
void mapDstIP(map<uint32_t, int> &dst_ip_map, uint32_t dst_ip);
void mapSrcPort(map<uint16_t, int> &src_port_map, uint32_t src_port);
void mapFlags(map<uint8_t, int> &flags_map, uint8_t flag);
void mapDstPort(map<uint16_t, int> &dst_port_map, uint32_t dst_port);
void mapProto(map<uint8_t, int> &protocols_map, uint8_t protocol);
string preDetectionInfo(queue <packet> &packet_queue, ofstream &packet_file);
string get_flags(uint8_t flag);
string log_packet(packet packet, ofstream &packet_file);
string get_protocol(uint8_t protocol);
string ip_int_to_string(uint32_t ip_as_integer);

#endif
