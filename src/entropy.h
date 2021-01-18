#ifndef ENTROPY_HEADER
#define ENTROPY_HEADER
#include <vector>
#include <map>
#include <string>
#include "packet.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

using namespace std;
void printSubEntropies(map<string, vector<double>> entropies_of_headers);
void accumulate_subwindow_entropies(packet &current_packet);
void accumulate_packets(packet &current_packet);
void sample_estimator(int sampling_amount);
void zero_entropy();
void mapSrcIP(map<uint, uint> &src_ip_map, uint src_ip);
void mapDstIP(map<uint, uint> &dst_ip_map, uint dst_ip);
void mapSrcPort(map<uint, uint> &src_port_map, uint src_port);
void mapFlags(map<uint, uint> &flags_map, uint flag);
void mapDstPort(map<uint, uint> &dst_port_map, uint dst_port);
void mapProto(map<uint, uint> &protocols_map, uint protocol);
void mapBytes(map<uint, uint> &packet_length_map, uint packet_length);
string get_flags(uint flag);
string log_packet(packet packet);
string get_protocol(uint protocol);
string ip_int_to_string(uint ip_as_integer);
map <string, map<uint, uint>> count_values_all_headers(vector <packet> &packet_vector);
double calcEntropy(vector <double> probs);
map<string, double> calcMultiEntropy(map<string, map<uint, uint>> header_maps);
vector<double> calcProb(map<uint, uint> header_value_counter);

#endif
