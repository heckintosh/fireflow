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
Capture *capture_interface = NULL;

int main(int argc, char* argv[]){
    signal(SIGINT, sigIntHandler);
    CLI::App app{"FIREFLOW: USTH ANTI_DDOS"};

    string interface = "ens33";
    string debuglog = "/tmp/fireflow-log.txt";
    string packetlog = "/tmp/packet-log.txt";
    double window = 10.0;                   //seconds
    double subwindow = 5.0;
    int estimator = 5;
    int max_sizelog = 1048576 * 10;
    int max_files = 3;
    app.add_option("-i,--interface", interface, "Capture interface")->required();
    app.add_option("-d", debuglog, "Dump to log file");
    app.add_option("-p", packetlog,  "Where to dump packet log");
    app.add_option("-w", window, "The amount of packet to read");
    app.add_option("-e", estimator, "The number of windows to be used to set target value, ucl and lcl");
    app.add_option("-s, --subwindow", subwindow, "The amount of packet to read");
    app.add_option("--maxsize", max_sizelog, "Max size of a log file");
    app.add_option("--maxfiles", max_files, "Max numbers of log files");
    CLI11_PARSE(app, argc, argv);

    capture_interface = new Capture(interface, debuglog, packetlog, window, subwindow, estimator, max_sizelog, max_files);
    auto exec_logger = spdlog::basic_logger_mt("exec_logger", (*capture_interface).debugpath);
    auto packet_logger = spdlog::rotating_logger_mt("packet_logger", (*capture_interface).packetpath, (*capture_interface).max_sizelog, (*capture_interface).max_files);
    exec_logger->set_pattern("[%d/%m/%Y %T.%e] [%^%l%$] %v");
    packet_logger->set_pattern("%v");
    capture_interface->start_pfring_capture();
}


void sigIntHandler(int signal) {
    std::cout << "\nExiting..." << std::endl;
    (*capture_interface).stop_pfring_capture();
    exit(signal);
}