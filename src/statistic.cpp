#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>
#include <numeric>
#include "packet.h"
#include "statistic.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
using namespace std;

map<string, double> calcMultiEntropy(map<string, map<uint, uint>> header_maps)
{

    map<string, double> entropy;
    for (const auto &outer_map_pair : header_maps)
    {
        entropy.insert({outer_map_pair.first, calcEntropy(calcProb(outer_map_pair.second))});
    }
    return entropy;
}

//given a vector of the probabilities of all values of a 
double calcEntropy(vector<double> probability)
{
    double size = probability.size();
    double total = 0;
    for (const auto &p : probability)
    {
        total += -(p * log(p) / log(size));
    }
    return total;
};

// given a dictionary which contains {header_value : the number of times a header value appear)
// output a vector of probability (header_value is uint)
vector<double> calcProb(map<uint, uint> header_value_counter)
{
    double total = 0;
    vector<double> probs;
    for (const auto &p : header_value_counter)
    {
        total += p.second;
    }
    for (const auto &q : header_value_counter)
    {
        probs.push_back(q.second / total);
    }
    return probs;
};

// ip_int_to_string(): Converts an 32-bit unsigned variable to IP string.
string ip_int_to_string(uint ip_as_integer)
{

    struct in_addr inaddr;
    char ip_as_string[INET_ADDRSTRLEN];

    inaddr.s_addr = ip_as_integer;
    inet_ntop(AF_INET, &inaddr, ip_as_string, INET_ADDRSTRLEN);

    return ip_as_string;
}

// extract_bit_value(): Get the value of nth bit of an 8-bit variable.
uint extract_bit_value(uint num, uint bit)
{
    if (bit > 0 && bit <= 8)
        return ((num >> (bit - 1)) & 1);
    else
        return 0;
}

// get_flags(): Get values of a TCP flag in string format: example: "RST, ACK"
string get_flags(uint flag)
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

// get_protocol(): Get protocol of the packet.
string get_protocol(uint protocol)
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

// print_binary(): Print the binary representations of the flags.
uint print_binary(uint flags)
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

/* log_packet(): Getting the essentials of a packet & write it to logfile/stringstream buffer. */
string log_packet(packet current_packet)
{
    string src_ip_as_string = ip_int_to_string(current_packet.src_ip);
    string dst_ip_as_string = ip_int_to_string(current_packet.dst_ip);

// Defining the content to write to stdout/packet log file
#define writeContent current_packet.ts.tv_sec << " " << current_packet.packetCounter << " "                                             \
                                              << src_ip_as_string << " " << current_packet.src_port << " "                              \
                                              << dst_ip_as_string << " " << current_packet.dst_port << " "                              \
                                              << get_protocol(current_packet.protocol) << " " << get_flags(current_packet.flags) << " " \
                                              << current_packet.length << current_packet.packet_payload_length                          \
                                              << "\n"

    // Write to a stringstream buffer
    stringstream buffer;
    buffer << writeContent;
    spdlog::get("packet_logger")->info(buffer.str());
    // Return the string
    return buffer.str();
}

void printSubEntropies(map<string, vector<double>> temp){
    for (const auto &map_pair : temp){
        cout << map_pair.first << ": ";
        for (const auto &entropy : map_pair.second){
            cout << entropy << " ";
        }
        cout << endl;
    }
}