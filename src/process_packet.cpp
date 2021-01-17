#include <iostream>
#include <sstream>
#include <queue>
#include <map>
#include <string>
#include <vector>
#include "statistic.h"
#include "capture.h"
#include "packet.h"
#include "process_packet.h"

using namespace std;
vector<packet> p_vector;
map<string, vector<double>> cusum_entropies;
vector<string> elements = {"src_ip", "src_port", "dst_port", "flags"};

// process_packet(): Processing the packet we captured.

void process_packet(packet &current_packet, bool out_of_window)
{
    //cout << log_packet(current_packet);
    p_vector.push_back(current_packet);
    if (out_of_window)
    {
        vector<double> temp;
        for (const auto &header_entropy_pair : calcMultiEntropy(count_values_all_headers(p_vector)))
        {
            if (!cusum_entropies.count(header_entropy_pair.first))
            {
                cusum_entropies.insert({header_entropy_pair.first, temp});
            }
            if (cusum_entropies.count(header_entropy_pair.first))
            {
                cusum_entropies[header_entropy_pair.first].push_back(header_entropy_pair.second);
            }
        }
        p_vector.clear();
        printSubEntropies(cusum_entropies);
    }
}

void process_no_packet()
{
    vector<double> temp;
    for (const auto &elem : elements){
        if (!cusum_entropies.count(elem)){
            cusum_entropies.insert({elem, temp});
        }
        if (cusum_entropies.count(elem)){
            cusum_entropies[elem].push_back(0.0);
        }
    }
    printSubEntropies(cusum_entropies);
}

// Example: src_ip has 192.166.221.111 appears 10 times
// 192.166.221.111 is represented as uint like all data
map<string, map<uint, uint>> count_values_all_headers(vector<packet> &packet_vector)
{
    map<uint, uint> src_ip_counter;
    map<uint, uint> src_port_counter;
    map<uint, uint> dst_port_counter;
    map<uint, uint> flags_counter;
    //map<uint, uint> protocol_counter;
    //map<uint, uint> total_bytes;
    //map<uint, uint> total_packets;
    int i = 0;
    while (i < packet_vector.size())
    {
        mapSrcIP(src_ip_counter, packet_vector[i].src_ip);
        mapSrcPort(src_port_counter, packet_vector[i].src_port);
        mapDstPort(dst_port_counter, packet_vector[i].dst_port);
        mapFlags(flags_counter, packet_vector[i].flags);
        //mapBytes(total_bytes, packet_vector[i].packet_payload_length);
        //mapProto(protocol_counter, packet_vector[i].protocol); // For information only
        i++;
    }
    map<string, map<uint, uint>> multimap = {{"src_ip", src_ip_counter}, {"src_port", src_port_counter}, {"dst_port", dst_port_counter}, {"flags", flags_counter}};
    return multimap;
}

void mapProto(map<uint, uint> &protocols_map, uint protocol)
{
    if (protocols_map.count(protocol))
    {
        protocols_map[protocol] += 1;
    }
    else if (!protocols_map.count(protocol))
    {
        protocols_map.insert({protocol, 1});
    }
}

void mapFlags(map<uint, uint> &flags_map, uint flag)
{
    if (flags_map.count(flag))
    {
        flags_map[flag] += 1;
    }
    else if (!flags_map.count(flag))
    {
        flags_map.insert({flag, 1});
    }
}

void mapSrcPort(map<uint, uint> &src_port_map, uint src_port)
{
    if (src_port_map.count(src_port))
    {
        src_port_map[src_port] += 1;
    }
    else if (!src_port_map.count(src_port))
    {
        src_port_map.insert({src_port, 1});
    }
}

void mapDstPort(map<uint, uint> &dst_port_map, uint dst_port)
{
    if (dst_port_map.count(dst_port))
    {
        dst_port_map[dst_port] += 1;
    }
    else if (!dst_port_map.count(dst_port))
    {
        dst_port_map.insert({dst_port, 1});
    }
}

void mapDstIP(map<uint, uint> &dst_ip_map, uint dst_ip)
{
    if (dst_ip_map.count(dst_ip))
    {
        dst_ip_map[dst_ip] += 1;
    }
    else if (!dst_ip_map.count(dst_ip))
    {
        dst_ip_map.insert({dst_ip, 1});
    }
}

void mapSrcIP(map<uint, uint> &src_ip_map, uint src_ip)
{
    if (src_ip_map.count(src_ip))
    {
        src_ip_map[src_ip] += 1;
    }
    else if (!src_ip_map.count(src_ip))
    {
        src_ip_map.insert({src_ip, 1});
    }
}

void mapBytes(map<uint, uint> &packet_length_map, uint packet_length)
{
    if (packet_length_map.count(packet_length))
    {
        packet_length_map[packet_length] += 1;
    }
    else if (!packet_length_map.count(packet_length))
    {
        packet_length_map.insert({packet_length, 1});
    }
}