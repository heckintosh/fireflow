#include <string>
#include "CLI11.hpp"
#include "capture.h"
#include "packet.h"
using namespace std;

int packet::packetCounter = 0;

int main(int argc, char* argv[]){
    
    CLI::App app{"FIREFLOW: USTH ANTI_DDOS"};

    string interface, logfile_path, packetfile_path;
    app.add_option("-i,--interface", interface, "Capture interface")->required();
    app.add_option("-l", logfile_path, "Dump to log file");
    app.add_option("-p", packetfile_path,  "Where to dump packet log");
    CLI11_PARSE(app, argc, argv);

    Capture capture_inteface(interface, logfile_path, packetfile_path);
    capture_inteface.init_logging();
    capture_inteface.start_pfring_capture();
}