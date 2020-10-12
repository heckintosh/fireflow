#include "log4cpp/Appender.hh"
#include "log4cpp/BasicLayout.hh"
#include "log4cpp/Category.hh"
#include "log4cpp/FileAppender.hh"
#include "log4cpp/Layout.hh"
#include "log4cpp/OstreamAppender.hh"
#include "log4cpp/PatternLayout.hh"
#include "log4cpp/Priority.hh"
#include <iostream>
#include <pfring.h>
#include <string>
#include <sys/types.h>
#include <packet.h>
using namespace std;

string interface = "eth0";
extern log4cpp::Category& logger;
bool pfring_loop(const char* dev);
void pfring_packet_process();
packet_pointer pfring_packet_ptr = NULL;
void pfring_capture(packet_pointer func_ptr) {
    logger << log4cpp::Priority::INFO << "PF_RING plugin started";
    pfring_packet_ptr = func_ptr;
    logger << log4cpp::Priority::INFO << "Interface:" << interface;
}

bool pfring_loop(const char* device){
    
}

void pfring_packet_process(){
    const char* device = interface.c_str();
    bool pfring_init_result = false;
    pfring_init_result = pfring_loop(device);
    if (!pfring_init_result) {
        // Internal error in PF_RING
        logger << log4cpp::Priority::ERROR << "PF_RING initilization failed, exit from program";
        exit(1);
    }
}
void pfring_stop_capture(){
    pfring_breakloop(p);
}