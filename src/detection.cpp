#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

struct PacketLogger
{
    int packetid;
    string src_ip;
    int src_port; /* Host IP */
    string dst_ip;
    int dst_port;
    string protocol;
    string flag;
};

//print map type
template <typename K, typename V>
void print_map(map<K, V> const &m)
{
    for (auto const &pair : m)
    {
        cout << "{" << pair.first << ": " << pair.second << "}\n";
    }
}

vector<PacketLogger> parsePackets(string file_path)
{
    ifstream packetfile("/tmp/packet_logger");
    string line, word;
    vector<PacketLogger> packet_list;
    while (getline(packetfile, line))
    {
        PacketLogger packet;
        istringstream packet_info(line);
        packet_info >> packet.packetid;
        packet_info >> packet.src_ip;
        packet_info >> packet.src_port;
        packet_info >> packet.dst_ip;
        packet_info >> packet.dst_port;
        packet_info >> packet.protocol;
        packet_info >> packet.flag;
        packet_list.push_back(packet);
    }
    return packet_list;
}

int countTotalPackets(vector<PacketLogger> &packet_list)
{
    return packet_list.size();
}

map<string, int> countPacketsPerIP(vector<PacketLogger> &packet_list)
{
    map<string, int> packetsPerIP;
    string currentIP;
    for (int i = 0; i < packet_list.size(); i++)
    {
        currentIP = packet_list[i].src_ip;
        if (packetsPerIP.count(currentIP) == 0)
        {
            packetsPerIP.insert(pair<string, int>(currentIP, 1));
        }
        else
        {
            packetsPerIP[currentIP] += 1;
        }
    }
    return packetsPerIP;
}

int main()
{
    vector<PacketLogger> packet_list;
    packet_list = parsePackets("/tmp/packet_logger");
    print_map(countPacketsPerIP(packet_list));
}