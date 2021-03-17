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
            spdlog::get("exec_logger")->warn("UP SIDE CUSUM IS LARGER THAN UCL: {} {}", map_pair.first, Upper[map_pair.first]);
            spdlog::get("exec_logger")->warn("DETECTED AT {}th SAMPLE", count);
        }
        if (Lower[map_pair.first] < CusumTask.getTargetValues()[map_pair.first] - map_pair.second)
        {
            spdlog::get("exec_logger")->warn("LOW SIDE CUSUM IS SMALLER THAN LCL : {} {}", map_pair.first, Lower[map_pair.first]);
            spdlog::get("exec_logger")->warn("DETECTED AT {}th SAMPLE", count);
        }
    }
    spdlog::get("exec_logger")->info("CUSUM VALUE IS RESETTED");
}