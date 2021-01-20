#include <iostream>
#include <sstream>
#include <queue>
#include <map>
#include <string>
#include <vector>
#include "capture.h"
#include "packet.h"
#include "entropy.h"
#include "cusum.h"

using namespace std;
vector<packet> p_vector;
map<string, vector<double>> entropies_of_headers;
vector<string> headers = {"src_ip", "src_port", "dst_port", "flags"};

void sample_estimator(int sampling_amount)
{
    map<string, vector<double>> samples = {};
    cout << "SAMPLING AMOUNT: " << sampling_amount << endl;
    for (const auto &map_pair : entropies_of_headers)
    {
        for (int i = 0; i < sampling_amount; ++i)
        {
            samples[map_pair.first].push_back(map_pair.second[i]);
        }
    }
    cout << "SAMPLE ESTIMATOR :: " << endl;
    for (const auto &map_pair : samples){
        cout << map_pair.first << ": " << endl;
        for (const auto &member : map_pair.second){
            cout << member << " ";
        }
        cout << endl;
    }
    //estimateSigma(samples);
}

void accumulate_subwindow_entropies(packet &current_packet)
{
    //cout << log_packet(current_packet);
    p_vector.push_back(current_packet);
    vector<double> temp;
    for (const auto &map_pair : calcMultiEntropy(count_values_all_headers(p_vector)))
    {
        entropies_of_headers[map_pair.first].push_back(map_pair.second);
    }
    p_vector.clear();
    printSubEntropies(entropies_of_headers);
}

void accumulate_packets(packet &current_packet)
{
    p_vector.push_back(current_packet);
}

void zero_entropy()
{
    vector<double> temp;
    for (const auto &elem : headers)
    {
        entropies_of_headers[elem].push_back(0.0);
    }
    printSubEntropies(entropies_of_headers);
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
        if (p == 1){
            return 0;
        }
        else{
            total += -(p * log(p) / log(size));
        }
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

void printSubEntropies(map<string, vector<double>> temp)
{
    for (const auto &map_pair : temp)
    {
        cout << map_pair.first << ": ";
        for (const auto &entropy : map_pair.second)
        {
            cout << entropy << " ";
        }
        cout << endl;
    }
}