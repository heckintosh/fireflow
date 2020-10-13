#ifndef CAPTURE_HEADER
#define CAPTURE_HEADER
#include <packet.h>
void pfring_capture(packet_process_ptr func_ptr);
#endif