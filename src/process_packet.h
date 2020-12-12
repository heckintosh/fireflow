#ifndef PROCESS_PACKET_HEADER

    #define PROCESS_PACKET_HEADER
    #include <string>
    #include "packet.h"

    using namespace std;

    void   process_packet  (packet   &current_packet, ofstream& packet_file, int &window);
    string log_packet      (packet   packet,          ofstream& packet_file);
    string ip_int_to_string(uint32_t ip_as_integer);

#endif
