#include "packet.h"
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator> 
#include <string>

#define TCP_FIN_FLAG_SHIFT 1
#define TCP_SYN_FLAG_SHIFT 2
#define TCP_RST_FLAG_SHIFT 3
#define TCP_PSH_FLAG_SHIFT 4
#define TCP_ACK_FLAG_SHIFT 5
#define TCP_URG_FLAG_SHIFT 6

using namespace std;
//Function declaration
void process_packet(packet &current_packet, string packet_file);
string ip_int_to_string(uint32_t ip_as_integer);
string log_packet(packet packet, string packet_file);

void process_packet(packet &current_packet, string packet_file)
{
    cout << log_packet(current_packet, packet_file);
}

string ip_int_to_string(uint32_t ip_as_integer)
{
    struct in_addr inaddr;
    inaddr.s_addr = ip_as_integer;
    char ip_as_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &inaddr, ip_as_string, INET_ADDRSTRLEN);
    return ip_as_string;
}

int extract_bit_value(uint8_t num, int bit) {
    if (bit > 0 && bit <= 8) {
        return ((num >> (bit - 1)) & 1);
    } else {
        return 0;
    }
}

string get_flags(uint8_t flag) {
    if (flag == 0) {
        return "-";
    }

    vector<string> tcp_flags;

    //I do not get the CWR and ECE flags because I do not think that it is necessary.

    if (extract_bit_value(flag, TCP_FIN_FLAG_SHIFT)) {
        tcp_flags.push_back("FIN");
    }

    if (extract_bit_value(flag, TCP_SYN_FLAG_SHIFT)) {
        tcp_flags.push_back("SYN");
    }

    if (extract_bit_value(flag, TCP_RST_FLAG_SHIFT)) {
        tcp_flags.push_back("RST");
    }

    if (extract_bit_value(flag, TCP_PSH_FLAG_SHIFT)) {
        tcp_flags.push_back("PSH");
    }

    if (extract_bit_value(flag, TCP_ACK_FLAG_SHIFT)) {
        tcp_flags.push_back("ACK");
    }

    if (extract_bit_value(flag, TCP_URG_FLAG_SHIFT)) {
        tcp_flags.push_back("URG");
    }

    ostringstream flags_as_string;

    if (tcp_flags.empty()) {
        return "-";
    }

    // concatenate all vector elements with comma
    copy(tcp_flags.begin(), tcp_flags.end() - 1, ostream_iterator<string>(flags_as_string, ","));

    // add last element
    flags_as_string << tcp_flags.back();

    return flags_as_string.str();
}
uint8_t print_binary(uint8_t flags) {
    // 0     0     0     0     0      0     0         0
    //CWR	ECE	  URG	ACK	  PSH	 RST	SYN	     FIN
    int mask = 128;
    while (mask != 1){
        if ((flags & mask) == mask) 
            cout << 1;
        else 
            cout << 0;
        mask = mask >> 1;
    }
    if ((flags & 1) == 1) cout << 1; else cout << 0;
    return 1;
}



string log_packet(packet current_packet, string packet_file)
{
    ofstream packetlog;
    string src_ip_as_string = ip_int_to_string(current_packet.src_ip);
    string dst_ip_as_string = ip_int_to_string(current_packet.dst_ip);
    //print to stdout
    stringstream buffer;
    buffer << current_packet.internalPacketCounter << " " << src_ip_as_string << " " << current_packet.src_port  << " " << dst_ip_as_string << " " << current_packet.dst_port << " " << get_flags(current_packet.flags) << " " << current_packet.length << "\n";

    //save to a file
    packetlog.open(packet_file,ios::app);
    if (packetlog.is_open())
    {
        packetlog << current_packet.internalPacketCounter << " " << src_ip_as_string << " " << current_packet.src_port  << " " << dst_ip_as_string << " " << current_packet.dst_port << " " << get_flags(current_packet.flags) << " " << current_packet.length << "\n";
        packetlog.close();
    }
    else{
        cout << "Cannot open file to log packet";
        exit(1);
    }
    return buffer.str();
}
