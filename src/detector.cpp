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
        if (Upper[map_pair.first] > map_pair.second)
        {
            spdlog::get("exec_logger")->warn("UPPER CUSUM IS LARGER THAN THRESHOLD: {} {}", map_pair.first, Upper[map_pair.first]);
        }
        if (Lower[map_pair.first] > map_pair.second)
        {
            spdlog::get("exec_logger")->warn("LOWER IS LARGER THAN THRESHOLD: {} {}", map_pair.first, Lower[map_pair.first]);
        }
    }
}