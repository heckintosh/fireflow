#ifndef PARSER_HEADER
#define PARSER_HEADER
#include <string>
#include "packet.h"
using namespace std;
string get_flags(uint8_t flag);
string log_packet(packet packet);
string get_protocol(uint8_t protocol);
string ip_int_to_string(uint32_t ip_as_integer);
string timevalToDate(struct timeval tv);

#endif