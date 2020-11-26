#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iterator>
#include <string>
#include "packet.h"
#include "process_packet.h"

#define TCP_FIN_FLAG_SHIFT 1
#define TCP_SYN_FLAG_SHIFT 2
#define TCP_RST_FLAG_SHIFT 3
#define TCP_PSH_FLAG_SHIFT 4
#define TCP_ACK_FLAG_SHIFT 5
#define TCP_URG_FLAG_SHIFT 6

using namespace std;

/*
    process_packet():
        Processing the packet we captured.
*/
void process_packet(packet &current_packet, string packet_file) {
    cout << log_packet(current_packet, packet_file);
}

/*
    ip_int_to_string():
        Converts an 32-bit unsigned variable to IP string.
*/
string ip_int_to_string(uint32_t ip_as_integer) {

    struct in_addr  inaddr;
    char            ip_as_string[INET_ADDRSTRLEN];

    inaddr.s_addr = ip_as_integer;
    inet_ntop(AF_INET, &inaddr, ip_as_string, INET_ADDRSTRLEN);

    return ip_as_string;

}

/*
    extract_bit_value():
        Get the value of nth bit of an 8-bit variable.
*/
int extract_bit_value(uint8_t num, int bit) {
    if (bit > 0 && bit <= 8) return ((num >> (bit - 1)) & 1);
    else                     return 0;
}

/*
    get_flags():
        Get values of a TCP flag in string format:
        example: "RST, ACK"
*/
string get_flags(uint8_t flag) {

    if (flag == 0)
        return "-";

    vector<string> tcp_flags;

    //I do not get the CWR and ECE flags because I do not think that it is necessary.
    if (extract_bit_value(flag, TCP_FIN_FLAG_SHIFT)) tcp_flags.push_back("FIN");
    if (extract_bit_value(flag, TCP_SYN_FLAG_SHIFT)) tcp_flags.push_back("SYN");
    if (extract_bit_value(flag, TCP_RST_FLAG_SHIFT)) tcp_flags.push_back("RST");
    if (extract_bit_value(flag, TCP_PSH_FLAG_SHIFT)) tcp_flags.push_back("PSH");
    if (extract_bit_value(flag, TCP_ACK_FLAG_SHIFT)) tcp_flags.push_back("ACK");
    if (extract_bit_value(flag, TCP_URG_FLAG_SHIFT)) tcp_flags.push_back("URG");
    if (tcp_flags.empty())
        return "-";

    // Concatenate all flags value in the vector, each item seperated by ','
    ostringstream flags_as_string;
    copy(tcp_flags.begin(), tcp_flags.end() - 1, ostream_iterator<string>(flags_as_string, ","));
    flags_as_string << tcp_flags.back(); // Add final item to the stream seperately to avoid ',' at the end

    return flags_as_string.str();

}

/*
    get_protocol():
        Get protocol of the packet.
*/
string get_protocol(uint8_t protocol) {
    if (protocol == IPPROTO_ICMP) return "ICMP";
    if (protocol == IPPROTO_TCP)  return "TCP";
    if (protocol == IPPROTO_UDP)  return "UDP";
    return "Unhandled, CODE: " + to_string(protocol);
}

/*
    print_binary():
        Print the binary representations of the flags.
*/
uint8_t print_binary(uint8_t flags) {

    // 0     0     0     0     0      0     0         0
    //CWR	ECE	  URG	ACK	  PSH	 RST	SYN	     FIN
    int mask = 128;
    while (mask != 1){
        if ((flags & mask) == mask) cout << 1;
        else                        cout << 0;
        mask = mask >> 1;
    }
    if ((flags & 1) == 1) cout << 1;
    else                  cout << 0;
    return 1;

}

/*
    log_packet():
        Getting the essentials of a packet & write it to logfile/stringstream buffer.
*/
string log_packet(packet current_packet, string packet_file)
{

    ofstream packetlog;
    string   src_ip_as_string = ip_int_to_string(current_packet.src_ip);
    string   dst_ip_as_string = ip_int_to_string(current_packet.dst_ip);

    // Defining the content to write to stdout/packet log file
    #define writeContent    packet::internalPacketCounter << " "                                             \
                         << src_ip_as_string << " " << current_packet.src_port << " "                               \
                         << dst_ip_as_string << " " << current_packet.dst_port << " "                               \
                         << get_protocol(current_packet.protocol) << " " << get_flags(current_packet.flags) << " "  \
                         << current_packet.length                                                                   \
                         << "\n"                                                                                    \

    // Write to a stringstream buffer
    stringstream buffer;
    buffer << writeContent;

    // Save to a file
    packetlog.open(packet_file, ios::app);
    if (packetlog.is_open()) {
        packetlog << writeContent;
        packetlog.close();
    }
    else {
        cout << "[!] Cannot open file to log packet!";
        exit(1);
    }

    // Return the string
    return buffer.str();

}
