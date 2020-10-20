#include "packet.h"
#include <sstream>
#include <arpa/inet.h>
using namespace std;

//Function declaration
void process_packet(packet& current_packet);
string ip_int_to_string(uint32_t ip_as_integer);
string print_simple_packet(packet packet);

void process_packet(packet& current_packet){
    cout << print_simple_packet(current_packet);
}

string ip_int_to_string(uint32_t ip_as_integer){
    struct in_addr inaddr;
    inaddr.s_addr = ip_as_integer;
    char ip_as_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &inaddr, ip_as_string, INET_ADDRSTRLEN);
    return ip_as_string;
}

string print_simple_packet(packet packet) {
    stringstream buffer;
    string source_ip_as_string = ip_int_to_string(packet.src_ip);
    buffer << source_ip_as_string;
    return buffer.str();
}



