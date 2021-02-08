#ifndef MAPPING_HEADER
#define MAPPING_HEADER
#include <map>
#include <vector>
#include "packet.h"
using namespace std;
void mapSrcIP(map<uint, uint> &src_ip_map, uint src_ip);
void mapDstIP(map<uint, uint> &dst_ip_map, uint dst_ip);
void mapSrcPort(map<uint, uint> &src_port_map, uint src_port);
void mapFlags(map<uint, uint> &flags_map, uint flag);
void mapDstPort(map<uint, uint> &dst_port_map, uint dst_port);
void mapProto(map<uint, uint> &protocols_map, uint protocol);
void mapBytes(map<uint, uint> &packet_length_map, uint packet_length);
map <string, map<uint, uint>> count_values_all_headers(vector <packet> &packet_vector);
void logSubwindowValues(int subwindow_counter, map<string, map<uint, uint>> multimap);
void mapLength(map<uint, uint> &length_map, uint length);
#endif