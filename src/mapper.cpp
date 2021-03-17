#include <algorithm>
#include "capture.h"
#include "mapper.h"
#include "parser.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
using namespace std;

//Use to declare which attributes are used to detect DDoS
Mapper::Mapper()
{   
    //Use Source IP?
    if (std::count(Capture::attributes.begin(), Capture::attributes.end(), "src_ip"))
    {
        Mapper::useSrcIP = true;
    }
    // Use Source Port?
    if (std::count(Capture::attributes.begin(), Capture::attributes.end(), "src_port"))
    {
        Mapper::useSrcPort = true;
    }
    // Use Source Flags?
    if (std::count(Capture::attributes.begin(), Capture::attributes.end(), "src_ip"))
    {
        Mapper::useFlags = true;
    }
}

// Count number of times the same values of a header appears, return them in a map
map<string, map<uint, uint>> Mapper::GetValCountAllHeaders(vector<packet> &packet_vector)
{
    map<uint, uint> src_ip_counter;
    map<uint, uint> src_port_counter;
    map<uint, uint> flags_counter;
    map<string, map<uint, uint>> multimap;
    int i = 0;
    while (i < packet_vector.size())
    {
        if (Mapper::useSrcIP)
        {
            mapSrcIP(src_ip_counter, packet_vector[i].src_ip);
        }
        if (Mapper::useSrcPort)
        {
            mapSrcPort(src_port_counter, packet_vector[i].src_port);
        }
        if (useFlags)
        {
            mapFlags(flags_counter, packet_vector[i].flags);
        }
        i++;
    }
    if (Mapper::useSrcIP)
    {
        multimap.insert({"src_ip", src_ip_counter});
    }
    if (Mapper::useSrcPort)
    {
        multimap.insert({"src_port", src_port_counter});
    }
    if (useFlags)
    {
        multimap.insert({"flags", src_port_counter});
    }
    return multimap;
}

void Mapper::logSubwindowValues(int subwindow_counter, map<string, map<uint, uint>> multimap)
{
    auto logger = spdlog::get("substat_logger");
    for (const auto &map_pair : multimap)
    {
        logger->info("{}: ", map_pair.first);
        for (const auto &header_counter : map_pair.second)
        {
            if (map_pair.first == "src_ip")
            {
                logger->info("{}: {}", ip_int_to_string(header_counter.first), header_counter.second);
            }
            else if (map_pair.first == "flags")
            {
                logger->info("{}: {}", get_flags(header_counter.first), header_counter.second);
            }
            else if (map_pair.first == "protocol")
            {
                logger->info(get_protocol(header_counter.first), header_counter.second);
            }
            else
            {
                logger->info("{}: {}", header_counter.first, header_counter.second);
            }
        }
    }
}
void Mapper::mapLength(map<uint, uint> &length_map, uint length)
{
    length_map[length] += 1;
}

void Mapper::mapProto(map<uint, uint> &protocols_map, uint protocol)
{
    protocols_map[protocol] += 1;
}

void Mapper::mapFlags(map<uint, uint> &flags_map, uint flag)
{
    flags_map[flag] += 1;
}

void Mapper::mapSrcPort(map<uint, uint> &src_port_map, uint src_port)
{
    src_port_map[src_port] += 1;
}

void Mapper::mapDstPort(map<uint, uint> &dst_port_map, uint dst_port)
{
    dst_port_map[dst_port] += 1;
}

void Mapper::mapDstIP(map<uint, uint> &dst_ip_map, uint dst_ip)
{
    dst_ip_map[dst_ip] += 1;
}

void Mapper::mapSrcIP(map<uint, uint> &src_ip_map, uint src_ip)
{
    src_ip_map[src_ip] += 1;
}

void Mapper::mapBytes(map<uint, uint> &packet_length_map, uint packet_length)
{
    packet_length_map[packet_length] += 1;
}

bool Mapper::getAttributeStatus(string attribute){
    if (attribute == "src_ip"){
        return Mapper::useSrcIP;
    }
    else if (attribute == "src_port"){
        return Mapper::useSrcPort;
    }
    else if (attribute == "flags"){
        return Mapper::useFlags;
    }
}