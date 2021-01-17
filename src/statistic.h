#ifndef ENTROPY_HEADER
#define ENTROPY_HEADER
#include <vector>
#include <map>
#include <string>
#include "packet.h"
using namespace std;
#define TCP_FIN_FLAG_SHIFT 1
#define TCP_SYN_FLAG_SHIFT 2
#define TCP_RST_FLAG_SHIFT 3
#define TCP_PSH_FLAG_SHIFT 4
#define TCP_ACK_FLAG_SHIFT 5
#define TCP_URG_FLAG_SHIFT 6
string log_packet(packet current_packet);
string ip_int_to_string(uint ip_as_integer);
string get_flags(uint flag);
string get_protocol(uint protocol);
void printSubEntropies(map<string, vector<double>> temp);
uint print_binary(uint flags);
uint extract_bit_value(uint num, uint bit);
double calcEntropy(vector <double> probs);
map<string, double> calcMultiEntropy(map<string, map<uint, uint>> header_maps);
vector<double> calcProb(map<uint, uint> header_value_counter);
#endif