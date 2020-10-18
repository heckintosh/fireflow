#include "packet.h"
#include <sstream>
using namespace std;

void process_packet(packet& current_packet){
    cout << print_simple_packet(current_packet);
}

string print_simple_packet(packet packet) {
    stringstream buffer;
    buffer << packet.src_ip << ":" << packet.source_port;
    return buffer.str();
}

