#include "detector.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
using namespace std;
void Detector::judge(Cusum CusumTask)
{
    map<string, double> Upper = CusumTask.getUpperCusum();
    map<string, double> Lower = CusumTask.getLowerCusum();
    map<string, double> Bound = CusumTask.getControlLimit();
    for (const auto &map_pair : Bound)
    {
        int count = CusumTask.getSubCount();
        if (Upper[map_pair.first] > CusumTask.getTargetValues()[map_pair.first] + map_pair.second)
        {
            cout << "DDOS incidents detected, check the log for more details" << endl;
            spdlog::get("alert_logger")->warn("A DDoS incident is detected at subgroup {}. Upper SUM is larger than UCL: {} {}", count, map_pair.first, Upper[map_pair.first]);
            CusumTask.logCusum(CusumTask.cusumTable, "cusum_logger");
            exit(1);
        }
        if (Lower[map_pair.first] < CusumTask.getTargetValues()[map_pair.first] - map_pair.second)
        {
            cout << "DDoS incidents detected, check the log for more details" << endl;
            spdlog::get("alert_logger")->warn("A DDoS incident is detected at subgroup {}. Lower SUM is smaller than LCL : {} {}", count, map_pair.first, Lower[map_pair.first]);
            CusumTask.logCusum(CusumTask.cusumTable, "cusum_logger");
            exit(1);
        }
    }
}