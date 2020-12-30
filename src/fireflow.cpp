#include <iostream>
#include <string>
#include <csignal>
#include "CLI11.hpp"
#include "capture.h"
#include "packet.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
using namespace std;

void sigIntHandler(int signal);
int packet::totalPackets = 0;

Capture *capture_interface = NULL;

int main(int argc, char* argv[]){
    signal(SIGINT, sigIntHandler);
    CLI::App app{"FIREFLOW: USTH ANTI_DDOS"};

    string interface = "eth0";
    string debuglog = "/tmp/fireflow-log.txt";
    string packetlog = "/tmp/debug-log.txt";
    int window = 1000;
    int max_sizelog = 1048576 * 10;
    int max_files = 3;
    app.add_option("-i,--interface", interface, "Capture interface")->required();
    app.add_option("-d", debuglog, "Dump to log file");
    app.add_option("-p", packetlog,  "Where to dump packet log");
    app.add_option("-w", window, "The amount of packet to read");
    app.add_option("--maxsize", max_sizelog, "Max size of a log file");
    app.add_option("--maxfiles", max_files, "Max numbers of log files");
    CLI11_PARSE(app, argc, argv);

    Capture capture_interface(interface, debuglog, packetlog, window, max_sizelog, max_files);
    auto exec_logger = spdlog::basic_logger_mt("exec_logger", capture_interface.debugpath);
    capture_interface.start_pfring_capture();
}


void sigIntHandler(int signal) {
    std::cout << "\nExiting..." << std::endl;
    (*capture_interface).stop_pfring_capture();
    exit(signal);
}