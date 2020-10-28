#include "packet_t.h"
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

//Function declaration
void process_packet(packet_t &current_packet);
string ip_int_to_string(uint32_t ip_as_integer);
string log_packet(packet_t packet);

void process_packet(packet_t &current_packet)
{
    cout << log_packet(current_packet);
}

string ip_int_to_string(uint32_t ip_as_integer)
{
    struct in_addr inaddr;
    inaddr.s_addr = ip_as_integer;
    char ip_as_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &inaddr, ip_as_string, INET_ADDRSTRLEN);
    return ip_as_string;
}

string log_packet(packet_t packet)
{
    ofstream packet_file;
    string src_ip_as_string = ip_int_to_string(packet.src_ip);
    string dst_ip_as_string = ip_int_to_string(packet.dst_ip);
    //print to stdout
    stringstream buffer;
    buffer << packet::number_of_packets << " " << src_ip_as_string << " " << dst_ip_as_string << "\n";

    //save to a file
    packet_file.open("/tmp/fireflow/packet_logger.txt",ios::app);
    if (packet_file.is_open())
    {
        packet_file << packet::number_of_packets << " " << src_ip_as_string << " " << dst_ip_as_string << "\n";
        packet_file.close();
    }
    else{
        cout << "Unable to open file";
    }
    return buffer.str();
}
