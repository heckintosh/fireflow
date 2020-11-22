#ifndef CAPTURE_HEADER
#define CAPTURE_HEADER
#include "packet.h"
#include <pfring.h>

void pfring_packet_process();
void pfring_capture(packet_process_ptr func_ptr);
void init_logging();
void parsing_pfring_packet(const struct pfring_pkthdr* h, const u_char* p, const u_char* user_bytes);
void start_pfring_capture();
void stop_pfring_capture();

bool start_pfring_packet_preprocessing(const char* dev);


#endif