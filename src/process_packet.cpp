#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <map>
#include <iterator>
#include <map>
#include <string>
#include <set>
#include <pfring.h>
#include "capture.h"
#include "packet.h"
#include "process_packet.h"

#define TCP_FIN_FLAG_SHIFT 1
#define TCP_SYN_FLAG_SHIFT 2
#define TCP_RST_FLAG_SHIFT 3
#define TCP_PSH_FLAG_SHIFT 4
#define TCP_ACK_FLAG_SHIFT 5
#define TCP_URG_FLAG_SHIFT 6

using namespace std;
queue<packet> packet_queue;

/*
    process_packet():
        Processing the packet we captured.
*/

void process_packet(packet current_packet)
{   
    if (Capture::window % 1000 == 0){
        cout << "test";
    }
}

string preDetectionInfo(queue <packet> &packet_queue, ofstream& packetlog){
    stringstream buffer;
    map<uint32_t, int> src_ip;
    map<uint32_t, int> dst_ip;
    map<uint16_t, int> src_port;
    map<uint16_t, int> dst_port;
    map<uint8_t, int> protocol;
    map<uint8_t, int> flags; 
    while (!packet_queue.empty()){
        buffer << log_packet(packet_queue.front(), packetlog);
        mapSrcIP(src_ip, packet_queue.front().src_ip); 
        mapDstIP(dst_ip, packet_queue.front().dst_ip);
        mapSrcPort(src_port, packet_queue.front().src_port);
        mapDstPort(dst_port, packet_queue.front().dst_port);
        mapProto(protocol, packet_queue.front().protocol);
        mapFlags(flags, packet_queue.front().flags);
        packet_queue.pop();
    }
    packetlog << buffer.str();
    return buffer.str();
}

/*
    log_packet():
        Getting the essentials of a packet & write it to logfile/stringstream buffer.

*/
string log_packet(packet current_packet, ofstream& packetlog)
{
    string src_ip_as_string = ip_int_to_string(current_packet.src_ip);
    string dst_ip_as_string = ip_int_to_string(current_packet.dst_ip);

    // Defining the content to write to stdout/packet log file
    #define writeContent    current_packet.packetCounter << " "                                             \
                         << src_ip_as_string << " " << current_packet.src_port << " "                               \
                         << dst_ip_as_string << " " << current_packet.dst_port << " "                               \
                         << get_protocol(current_packet.protocol)  << " " << get_flags(current_packet.flags) << " "  \
                         << current_packet.length                                                                   \
                         << "\n"                                                                                    \

    // Write to a stringstream buffer
    stringstream buffer;
    buffer << writeContent;   

    // Return the string
    return buffer.str();
}

void mapProto(map<uint8_t, int> &protocols_map, uint8_t protocol){
    if (protocols_map.count(protocol)){
        protocols_map[protocol] += 1;
    }
    else if (!protocols_map.count(protocol)){
        protocols_map.insert({protocol, 1});
    }
}

void mapFlags(map<uint8_t, int> &flags_map, uint8_t flag){
    if (flags_map.count(flag)){
        flags_map[flag] += 1;
    }
    else if (!flags_map.count(flag)){
        flags_map.insert({flag, 1});
    }
}

void mapSrcPort(map<uint16_t, int> &src_port_map, uint32_t src_port){
    if (src_port_map.count(src_port)){
        src_port_map[src_port] += 1;
    }
    else if (!src_port_map.count(src_port)){
        src_port_map.insert({src_port, 1});
    }
}

void mapDstPort(map<uint16_t, int> &dst_port_map, uint32_t dst_port){
    if (dst_port_map.count(dst_port)){
        dst_port_map[dst_port] += 1;
    }
    else if (!dst_port_map.count(dst_port)){
        dst_port_map.insert({dst_port, 1});
    }
}

void mapDstIP(map<uint32_t, int> &dst_ip_map, uint32_t dst_ip){
    if (dst_ip_map.count(dst_ip)){
        dst_ip_map[dst_ip] += 1;
    }
    else if (!dst_ip_map.count(dst_ip)){
        dst_ip_map.insert({dst_ip, 1});
    }
}

void mapSrcIP(map<uint32_t, int> &src_ip_map, uint32_t src_ip){
    if (src_ip_map.count(src_ip)){
        src_ip_map[src_ip] += 1;
    }
    else if (!src_ip_map.count(src_ip)){
        src_ip_map.insert({src_ip, 1});
    }
}

/*
    ip_int_to_string():
        Converts an 32-bit unsigned variable to IP string.
*/
string ip_int_to_string(uint32_t ip_as_integer)
{

    struct in_addr inaddr;
    char ip_as_string[INET_ADDRSTRLEN];

    inaddr.s_addr = ip_as_integer;
    inet_ntop(AF_INET, &inaddr, ip_as_string, INET_ADDRSTRLEN);

    return ip_as_string;
}

/*
    extract_bit_value():
        Get the value of nth bit of an 8-bit variable.
*/
int extract_bit_value(uint8_t num, int bit)
{
    if (bit > 0 && bit <= 8)
        return ((num >> (bit - 1)) & 1);
    else
        return 0;
}

/*
    get_flags():
        Get values of a TCP flag in string format:
        example: "RST, ACK"
*/
string get_flags(uint8_t flag)
{

    if (flag == 0)
        return "-";

    vector<string> tcp_flags;

    //I do not get the CWR and ECE flags because I do not think that it is necessary.
    if (extract_bit_value(flag, TCP_FIN_FLAG_SHIFT))
        tcp_flags.push_back("FIN");
    if (extract_bit_value(flag, TCP_SYN_FLAG_SHIFT))
        tcp_flags.push_back("SYN");
    if (extract_bit_value(flag, TCP_RST_FLAG_SHIFT))
        tcp_flags.push_back("RST");
    if (extract_bit_value(flag, TCP_PSH_FLAG_SHIFT))
        tcp_flags.push_back("PSH");
    if (extract_bit_value(flag, TCP_ACK_FLAG_SHIFT))
        tcp_flags.push_back("ACK");
    if (extract_bit_value(flag, TCP_URG_FLAG_SHIFT))
        tcp_flags.push_back("URG");
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
string get_protocol(uint8_t protocol)
{
    if (protocol == IPPROTO_ICMP)
        return "ICMP";
    else if (protocol == IPPROTO_TCP)
        return "TCP";
    else if (protocol == IPPROTO_UDP)
        return "UDP";
    else
        return "-";
}

/*
    print_binary():
        Print the binary representations of the flags.
*/
uint8_t print_binary(uint8_t flags)
{

    // 0     0     0     0     0      0     0         0
    //CWR	ECE	  URG	ACK	  PSH	 RST	SYN	     FIN
    int mask = 128;
    while (mask != 1)
    {
        if ((flags & mask) == mask)
            cout << 1;
        else
            cout << 0;
        mask = mask >> 1;
    }
    if ((flags & 1) == 1)
        cout << 1;
    else
        cout << 0;
    return 1;
}

