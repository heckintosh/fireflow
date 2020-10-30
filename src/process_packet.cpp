#include "packet.h"
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
//Function declaration
void process_packet(packet &current_packet);
string ip_int_to_string(uint32_t ip_as_integer);
string log_packet(packet packet);

void process_packet(packet &current_packet)
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

int extract_bit_value(uint8_t num, int bit) {
    if (bit > 0 && bit <= 8) {
        return ((num >> (bit - 1)) & 1);
    } else {
        return 0;
    }
}

string get_flags(uint8_t flag) {
    if (flag == 0) {
        return "-";
    }

    // taken from pfring.h
    // (tcp->fin * TH_FIN_MULTIPLIER) + (tcp->syn * TH_SYN_MULTIPLIER) +
    // (tcp->rst * TH_RST_MULTIPLIER) + (tcp->psh * TH_PUSH_MULTIPLIER) +
    // (tcp->ack * TH_ACK_MULTIPLIER) + (tcp->urg * TH_URG_MULTIPLIER);

    /*
        // Required for decoding tcp flags
        #define TH_FIN_MULTIPLIER   0x01
        #define TH_SYN_MULTIPLIER   0x02
        #define TH_RST_MULTIPLIER   0x04
        #define TH_PUSH_MULTIPLIER  0x08
        #define TH_ACK_MULTIPLIER   0x10
        #define TH_URG_MULTIPLIER   0x20
    */

    vector<string> all_flags;

    if (extract_bit_value(flag_value, TCP_FIN_FLAG_SHIFT)) {
        all_flags.push_back("fin");
    }

    if (extract_bit_value(flag_value, TCP_SYN_FLAG_SHIFT)) {
        all_flags.push_back("syn");
    }

    if (extract_bit_value(flag_value, TCP_RST_FLAG_SHIFT)) {
        all_flags.push_back("rst");
    }

    if (extract_bit_value(flag_value, TCP_PSH_FLAG_SHIFT)) {
        all_flags.push_back("psh");
    }

    if (extract_bit_value(flag_value, TCP_ACK_FLAG_SHIFT)) {
        all_flags.push_back("ack");
    }

    if (extract_bit_value(flag_value, TCP_URG_FLAG_SHIFT)) {
        all_flags.push_back("urg");
    }


    std::ostringstream flags_as_string;

    if (all_flags.empty()) {
        return "-";
    }

    // concatenate all vector elements with comma
    std::copy(all_flags.begin(), all_flags.end() - 1, std::ostream_iterator<std::string>(flags_as_string, ","));

    // add last element
    flags_as_string << all_flags.back();

    return flags_as_string.str();
}

string log_packet(packet current_packet)
{
    ofstream packet_file;
    string src_ip_as_string = ip_int_to_string(current_packet.src_ip);
    string dst_ip_as_string = ip_int_to_string(current_packet.dst_ip);
    //print to stdout
    stringstream buffer;
    buffer << current_packet.internalPacketCounter << " " << src_ip_as_string << ":" << current_packet.src_port  << " " << dst_ip_as_string << ":" << current_packet.dst_port << "\n";

    //save to a file
    packet_file.open("/tmp/fireflow/packet_logger.txt",ios::app);
    if (packet_file.is_open())
    {
        packet_file << current_packet.internalPacketCounter << " " << src_ip_as_string << ":" << current_packet.src_port  << " " << dst_ip_as_string << ":" << current_packet.dst_port << " " << current_packet.flags << "\n";
        packet_file.close();
    }
    else{
        cout << "Unable to open file";
    }
    return buffer.str();
}
