#include <iostream>
#include <string>
#include <csignal>
#include "CLI11.hpp"
#include "capture.h"
#include "packet.h"
using namespace std;

void sigIntHandler(int signal);
int packet::packetCounter = 0;

Capture *capture_inteface = NULL;

int main(int argc, char* argv[]){
    signal(SIGINT, sigIntHandler);
    CLI::App app{"FIREFLOW: USTH ANTI_DDOS"};

    string interface, logfile_path, packetfile_path;
    app.add_option("-i,--interface", interface, "Capture interface")->required();
    app.add_option("-l", logfile_path, "Dump to log file");
    app.add_option("-p", packetfile_path,  "Where to dump packet log");
    CLI11_PARSE(app, argc, argv);

    capture_inteface = new Capture(interface, logfile_path, packetfile_path);
    (*capture_inteface).init_logging();
    (*capture_inteface).start_pfring_capture();
}


void sigIntHandler(int signal) {
    std::cout << "\nExiting..." << std::endl;
    (*capture_inteface).stop_pfring_capture();
    exit(signal);
}