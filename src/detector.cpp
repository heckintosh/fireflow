#include "detector.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
using namespace std;
void Detector::judgeCusum(Cusum CusumTask)
{
    map<string, double> Upper = CusumTask.getUpperCusum();
    map<string, double> Lower = CusumTask.getLowerCusum();
    map<string, double> Bound = CusumTask.getControlLimit();
    for (const auto &map_pair : Bound)
    {
        if (Upper[map_pair.first] > CusumTask.getTargetValues()[map_pair.first] + map_pair.second)
        {
            spdlog::get("exec_logger")->warn("UP SIDE CUSUM IS LARGER THAN UCL: {} {}", map_pair.first, Upper[map_pair.first]);
            spdlog::get("exec_logger")->warn("DETECTED AT {}th SAMPLE", CusumTask.getSampleCounter());
            spdlog::get("exec_logger")->warn("THE DDOS ATTACK IS GUESSED TO HAVE STARTED AT SAMPLE: {}", CusumTask.getSampleCounter() - CusumTask.getShiftCounterUpper()[map_pair.first]);
            CusumTask.reset("Upper", map_pair.first);
        }
        if (Lower[map_pair.first] < CusumTask.getTargetValues()[map_pair.first] - map_pair.second)
        {
            spdlog::get("exec_logger")->warn("LOW SIDE CUSUM IS SMALLER THAN LCL : {} {}", map_pair.first, Lower[map_pair.first]);
            spdlog::get("exec_logger")->warn("DETECTED AT {}th SAMPLE", CusumTask.getSampleCounter());
            spdlog::get("exec_logger")->warn("THE DDOS ATTACK IS GUESSED TO HAVE STARTED AT SAMPLE: {}", CusumTask.getSampleCounter() - CusumTask.getShiftCounterLower()[map_pair.first]);
            CusumTask.reset("Lower", map_pair.first);
        }
    }
    spdlog::get("exec_logger")->info("CUSUM VALUE IS RESETTED");
}