#include "mapping.h"

// Example: src_ip has 192.166.221.111 appears 10 times
// 192.166.221.111 is represented as uint like all data
map<string, map<uint, uint>> count_values_all_headers(vector<packet> &packet_vector)
{
    map<uint, uint> src_ip_counter;
    map<uint, uint> src_port_counter;
    map<uint, uint> dst_port_counter;
    map<uint, uint> flags_counter;
    int i = 0;
    while (i < packet_vector.size())
    {
        mapSrcIP(src_ip_counter, packet_vector[i].src_ip);
        mapSrcPort(src_port_counter, packet_vector[i].src_port);
        mapDstPort(dst_port_counter, packet_vector[i].dst_port);
        mapFlags(flags_counter, packet_vector[i].flags);
        i++;
    }
    map<string, map<uint, uint>> multimap = {{"src_ip", src_ip_counter}, {"src_port", src_port_counter}, {"dst_port", dst_port_counter}, {"flags", flags_counter}};
    return multimap;
}

void mapProto(map<uint, uint> &protocols_map, uint protocol)
{
    protocols_map[protocol] += 1;
}

void mapFlags(map<uint, uint> &flags_map, uint flag)
{
    flags_map[flag] += 1;
}

void mapSrcPort(map<uint, uint> &src_port_map, uint src_port)
{
    src_port_map[src_port] += 1;
}

void mapDstPort(map<uint, uint> &dst_port_map, uint dst_port)
{
    dst_port_map[dst_port] += 1;
}

void mapDstIP(map<uint, uint> &dst_ip_map, uint dst_ip)
{
    dst_ip_map[dst_ip] += 1;
}

void mapSrcIP(map<uint, uint> &src_ip_map, uint src_ip)
{
    src_ip_map[src_ip] += 1;
}

void mapBytes(map<uint, uint> &packet_length_map, uint packet_length)
{
    packet_length_map[packet_length] += 1;
}