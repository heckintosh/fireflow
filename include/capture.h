#ifndef CAPTURE_HEADER
#define CAPTURE_HEADER
#include <string>
#include <pfring.h>
#include <vector>
#include "packet.h"
using namespace std;

class Capture
{
private:
    pfring *ring = NULL; // PF_RING socket to capture data

public:
    static unsigned int total_unparsed_packets;
    static int subwindow_counter;
    static double window;
    static int estimator;
    static double subwindow;
    static vector<string> attributes;
    unsigned int pfring_sampling_rate; // Sample rate (packets/second?)
    string interface;  // The ethernet interface to capture packet

    /* Constructor
        - interface 
        - pfring init log path
        - packet info log path
        - window: amount of packets to perform analysis on each
        - max_size: 10mb
        - max_files: 3
    */
    Capture(string _interface, double _window, double _subwindow, int _estimator, vector<string> &_attributes);

    //parsing_pfring_packet(): oosw = out of subwindow; oow = out of window
    static packet parsing_pfring_packet(const struct pfring_pkthdr *header, const u_char *buffer);
    static void parsing_pfring_packet_sw(const struct pfring_pkthdr *header, const u_char *buffer);
    static void parsing_pfring_packet_oosw(const struct pfring_pkthdr *header, const u_char *buffer);
    static void execution_flow(const struct pfring_pkthdr &header, const u_char *buffer, int hasPkt);
    static void log_packet(packet current_packet);
   // start_pfring_capture(): Choose an ethernet interface to capture, set sampling rate(?).
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
