#include "capture.h"
#include "mapping.h"
#include "parser.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

// Example: src_ip has 192.166.221.111 appears 10 times
// 192.166.221.111 is represented as uint like all data
map<string, map<uint, uint>> count_values_all_headers(vector<packet> &packet_vector)
{
    map<uint, uint> src_ip_counter;
    map<uint, uint> dst_ip_counter;
    map<uint, uint> src_port_counter;
    map<uint, uint> dst_port_counter;
    map<uint, uint> length_counter;
    map<uint, uint> flags_counter;
    map<uint, uint> protocol;
    int i = 0;
    while (i < packet_vector.size())
    {
        mapSrcIP(src_ip_counter, packet_vector[i].src_ip);
        mapDstIP(dst_ip_counter, packet_vector[i].dst_ip);
        mapSrcPort(src_port_counter, packet_vector[i].src_port);
        mapDstPort(dst_port_counter, packet_vector[i].dst_port);
        mapLength(length_counter, packet_vector[i].length);
        mapProto(protocol, packet_vector[i].protocol);
        mapFlags(flags_counter, packet_vector[i].flags);
        i++;
    }
    map<string, map<uint, uint>> multimap = {{"src_ip", src_ip_counter}, {"dst_ip", dst_ip_counter}, {"src_port", src_port_counter}, {"dst_port", dst_port_counter}, {"protocol", protocol}, {"flags", flags_counter}, {"length", length_counter}};
    logSubwindowValues(Capture::subwindow, multimap);
    return multimap;
}

void logSubwindowValues(int subwindow_counter, map<string, map<uint, uint>> multimap){
    auto logger = spdlog::get("substat_logger");
    for (const auto &map_pair : multimap){
        logger->info("{}: ", map_pair.first);
        for (const auto &header_counter : map_pair.second){
            if (map_pair.first == "src_ip"){
                logger->info("{}: {}",ip_int_to_string(header_counter.first), header_counter.second);
            }
            else if (map_pair.first == "flags"){
                logger->info("{}: {}", get_flags(header_counter.first), header_counter.second);
            }
            else if (map_pair.first == "protocol"){
                logger->info(get_protocol(header_counter.first), header_counter.second);
            }
            else{
                logger->info("{}: {}", header_counter.first, header_counter.second);
            }
        }
    }
}
void mapLength(map<uint, uint> &length_map, uint length)
{
    length_map[length] += 1;
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

