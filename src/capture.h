#ifndef CAPTURE_HEADER
#define CAPTURE_HEADER
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "capture.h"
#include <string>
#include <queue>
#include <pfring.h>
#include "packet.h"
using namespace std;

class Capture
{
    pfring *ring = NULL; // PF_RING socket to capture data

public:
    static unsigned int total_unparsed_packets;
    static double window;
    static double subwindow;
    string *packet_file_ptr;       // Just a pointer to the packet file... (maybe if set NULL to then file is shit?)
    unsigned int pfring_sampling_rate; // Sample rate (packets/second?)
    int max_sizelog;
    int max_files;
    string debugpath;  // Fireflow execution logger
    string interface;  // The ethernet interface to capture packet
    string packetpath; // The file contains packet's content

    /* Constructor
        - interface 
        - pfring init log path
        - packet info log path
        - window: amount of packets to perform analysis on each
        - max_size: 10mb
        - max_files: 3
    */
    Capture(string _interface, string _debugpath, string _packetpath, double _window, double _subwindow, int _max_sizelog, int _max_files);

    /*
        start_pfring_capture():
            Choose an ethernet interface to capture, set sampling rate(?).
    */
    static void parsing_pfring_packet(const struct pfring_pkthdr *header, const u_char *buffer, bool out_of_window, bool hasPkt);
    void start_pfring_capture();

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
