#include <iostream>
#include <string>
#include <csignal>
#include "CLI11.hpp"
#include "json.hpp"
#include "capture.h"
#include "mapper.h"
#include "packet.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
using namespace std;
using json = nlohmann::json;

void sigIntHandler(int signal);
Capture *capture_interface = NULL;

int main(int argc, char *argv[])
{
    signal(SIGINT, sigIntHandler);
    string interface, debuglog, packetlog, cusumlog, substatlog, thresholdlog, alertlog = "";
    vector<string> attributes;
    double window, subwindow = 0;
    int estimator, max_files, max_sizelog = 0;
    std::ifstream config("../configs/config.json");
    if (config.is_open())
    {
        json j;
        config >> j;
        interface = j["interface"];
        debuglog = j["debugpath"];
        packetlog = j["packetpath"];
        cusumlog = j["cusumpath"];
        thresholdlog = j["thresholdpath"];
        alertlog = j["alertpath"];
        window = j["window"]; //seconds
        subwindow = j["subwindow"];
        estimator = j["estimator"];
        max_sizelog = j["max_sizelog"];
        max_files = j["max_files"];
        for (int i = 0; i < j["attributes"].size(); i++){
            attributes.push_back(j["attributes"][i]);
        }
        config.close();
    }
    else{
        cout << "Failed to read config file.\n";
        exit(1);
    }
    capture_interface = new Capture(interface, window, subwindow, estimator, attributes);
    auto exec_logger = spdlog::rotating_logger_mt("exec_logger", debuglog, max_sizelog, max_files);
    auto packet_logger = spdlog::rotating_logger_mt("packet_logger",  packetlog, max_sizelog, max_files);
    auto cusum_logger = spdlog::rotating_logger_mt("cusum_logger",  cusumlog, max_sizelog, max_files);
    auto threshold_logger = spdlog::rotating_logger_mt("threshold_logger",  thresholdlog, max_sizelog, max_files);
    auto alert_logger = spdlog::rotating_logger_mt("alert_logger",  alertlog, max_sizelog, max_files);
    exec_logger->set_pattern("[%d/%m/%Y %T.%e] [%^%l%$] %v");
    cusum_logger->set_pattern("[%d/%m/%Y %T.%e] %v");
    packet_logger->set_pattern("[%d/%m/%Y %T.%e] %v");
    threshold_logger->set_pattern("%v");
    capture_interface->start_pfring_capture();
}

void sigIntHandler(int signal)
{
    std::cout << "\nExiting..." << std::endl;
    (*capture_interface).stop_pfring_capture();
    exit(signal);
}