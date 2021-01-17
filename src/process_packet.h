#ifndef PROCESS_PACKET_HEADER

#define PROCESS_PACKET_HEADER
#include <vector>
#include <map>
#include <string>
#include "packet.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;
void process_packet(packet &current_packet, bool out_of_window);
void mapSrcIP(map<uint, uint> &src_ip_map, uint src_ip);
void mapDstIP(map<uint, uint> &dst_ip_map, uint dst_ip);
void mapSrcPort(map<uint, uint> &src_port_map, uint src_port);
void mapFlags(map<uint, uint> &flags_map, uint flag);
void mapDstPort(map<uint, uint> &dst_port_map, uint dst_port);
void mapProto(map<uint, uint> &protocols_map, uint protocol);
void mapBytes(map<uint, uint> &packet_length_map, uint packet_length);
void printSubEntropies(map<string, vector<double>> cusum_entropies);
void process_no_packet();
string get_flags(uint flag);
string log_packet(packet packet);
string get_protocol(uint protocol);
string ip_int_to_string(uint ip_as_integer);
map <string, map<uint, uint>> count_values_all_headers(vector <packet> &packet_vector);

#endif
