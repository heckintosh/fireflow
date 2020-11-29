#ifndef CAPTURE_HEADER
#define CAPTURE_HEADER
#include <log4cpp/Appender.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>
#include <string>
#include <pfring.h>

#include "packet.h"
using namespace std;

class Capture
{

    log4cpp::Category &logger = log4cpp::Category::getRoot(); // The ultimiate logger!! Used to add settings etc...
    pfring *ring = NULL;                    // PF_RING socket to capture data

public:
    static string logfile_path; // Path to log file
    static string interface;                        // The ethernet interface to capture packet
    static string packet_file;    // The file contains packet's content
    
    static string *packet_file_ptr; // Just a pointer to the packet file... (maybe if set NULL to then file is shit?)
    static uint32_t pfring_sampling_rate;    // Sample rate (packets/second?)
    static uint64_t total_unparsed_packets;

    /*
        init_logging():
            Initialize the logging library.
    */
    Capture(string u_iface, string u_ringlog, string u_packlog);
    void init_logging();

    /*
        start_pfring_capture():
            Choose an ethernet interface to capture, set sampling rate(?).
    */

    void start_pfring_capture();

    /*
        parsing_pfring_packet():
            Parsing PF_RING packet.
    */

    static void parsing_pfring_packet(const struct pfring_pkthdr *header, const u_char *buffer, const u_char *user_bytes);
    
    /*
    start_pfring_packet_preprocessing():
        Intialize PF_RING variables and start capturing;
        Get info about PF_RING version;
        Set application name to 'fireflow';
        etc...
        [Args:] const char* dev: Name of the device we want to capture.
    */
    bool start_pfring_packet_preprocessing(const char *dev);
     /*
        stop_pfring_capture():
            Shuts down PF_RING capture.
    */

    void stop_pfring_capture();
};

#endif
