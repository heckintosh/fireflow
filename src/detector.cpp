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
    map<string, double> UpperBound = CusumTask.getUCL();
    map<string, double> LowerBound = CusumTask.getLCL();
    for (const auto &map_pair : UpperBound)
    {
        if (Upper[map_pair.first] > map_pair.second)
        {
            spdlog::get("exec_logger")->warn("HIGHER THAN UPPER THRESHOLD: {}", map_pair.first);
        }
    }
    for (const auto &map_pair : LowerBound)
    {
        if (Upper[map_pair.first] > map_pair.second)
        {
            spdlog::get("exec_logger")->warn("LOWER THAN LOWER THRESHOLD: {}", map_pair.first);
        }
    }
}