#ifndef CAPTURE_HEADER
#define CAPTURE_HEADER
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <string>
#include <queue>
#include "PFring.h"
#include "packet.h"
using namespace std;

class Capture
{
    PFring *ring = NULL; // PF_RING socket to capture data

public:
    static string *packet_file_ptr;       // Just a pointer to the packet file... (maybe if set NULL to then file is shit?)
    static uint32_t pfring_sampling_rate; // Sample rate (packets/second?)
    static uint64_t total_unparsed_packets;
    static queue<packet> packet_queue;
    int max_sizelog;
    int max_files;
    string debugpath;  // Fireflow execution logger
    string interface;  // The ethernet interface to capture packet
    string packetpath; // The file contains packet's content
    int window;
    static std::shared_ptr<spdlog::logger> exec_logger;
    static std::shared_ptr<spdlog::logger> packet_logger;

    /* Constructor
        - interface 
        - pfring init log path
        - packet info log path
        - window: amount of packets to perform analysis on each
        - max_size: 10mb
        - max_files: 3
    */
    Capture(string _interface = "eth0", string _debugpath = "/tmp/fireflow_log.txt", string _packetpath = "/tmp/packet_log.txt", int _window = 1000, int _max_sizelog = 1048576 * 10, int _max_files = 3);
    /*
        start_pfring_capture():
            Choose an ethernet interface to capture, set sampling rate(?).
    */

    void start_pfring_capture();

    /*
        parsing_pfring_packet():
            Parsing PF_RING packet.
    */
    void parsing_pfring_packet(const struct pfring_pkthdr *header, const u_char *buffer, const u_char *user_bytes);

    /*
    start_pfring_packet_preprocessing():
        Intialize PF_RING variables and start capturing;
        Get info about PF_RING version;
        Set application name to 'fireflow';
        etc...
        [Args:] const char* dev: Name of the device we want to capture.
    */
    bool start_pfring_packet_preprocessing(char *dev);
    /*
        stop_pfring_capture():
            Shuts down PF_RING capture.
    */

    void stop_pfring_capture();
};

#endif
