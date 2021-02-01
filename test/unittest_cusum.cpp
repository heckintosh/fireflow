#include <gtest/gtest.h>
#include "cusum.h"
Cusum CusumTask;
double error_margin = 0.000001;
vector<double> entropies = {0.96023, 0.58102, 0.20100, 0.31010};
map<string, vector<double>> samples = {{"src_ip", entropies}, {"dst_ip", entropies}};
TEST(CusumTest, TargetValues)
{
    double mean = 0.5130875;
    map<string, double> tmp = {{"src_ip", mean}, {"dst_ip", mean}};
    for (const auto &map_pair : CusumTask.getTargetValues())
    {
        ASSERT_NEAR(tmp[map_pair.first], map_pair.second, error_margin);
    }
}

int main()
{
    CusumTask.setSampleSize(4);
    CusumTask.setSubGroupSize(2);
    CusumTask.setThreshold(samples);
}