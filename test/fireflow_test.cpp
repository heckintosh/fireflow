#include <gtest/gtest.h>
#include "entropy.h"
#include "cusum.h"
double error_margin = 0.000001;

// DATA FOR ENTROPY TEST
vector<double> Probabilities = {(double)2 / 5, (double)1 / 5, (double)2 / 5};
double normalized_entropy = 0.96023;
map<uint, uint> header_counters = {{123, 2}, {1234, 1}, {12345, 2}};
map<string, double> header_entropies = {{"src_ip", normalized_entropy}, {"dst_ip", normalized_entropy}};
map<string, map<uint, uint>> header_maps = {{"src_ip", header_counters}, {"dst_ip", header_counters}};

// DATA FOR CUSUM
vector<double> cusum_data = {0.96023, 0.58102, 0.20100, 0.31010}; // This is actually random entropy data
map<string, vector<double>> samples = {{"src_ip", cusum_data}, {"dst_ip", cusum_data}};

//init
EntropyCalc EntropyTask;
Cusum CusumTask;

TEST(EntropyTest, Probability)
{
    ASSERT_EQ(Probabilities, EntropyTask.calcProb(header_counters));
}
TEST(EntropyTest, Entropy)
{
    ASSERT_NEAR(normalized_entropy, EntropyTask.calcEntropy(EntropyTask.calcProb(header_counters)), error_margin);
}

TEST(EntropyTest, MultiEntropy)
{
    map<string, double> tmp;
    tmp = EntropyTask.calcMultiEntropy(header_maps);
    for (const auto &map_pair : tmp)
    {
        ASSERT_NEAR(map_pair.second, header_entropies[map_pair.first], error_margin);
    }
}

TEST(CusumTest, TargetValues)
{
    //big mean of entropies vector;
    double mean = 0.5130875;
    map<string, double> tmp = {{"src_ip", mean}, {"dst_ip", mean}};
    for (const auto &map_pair : CusumTask.getTargetValues())
    {
        ASSERT_NEAR(tmp[map_pair.first], map_pair.second, error_margin);
    }
}

TEST(CusumTest, Mean)
{
    //big mean of entropies vector;
    double mean = 0.5130875;
    ASSERT_EQ(mean, CusumTask.calcMean(cusum_data));
}

TEST(CusumTest, DeviationOfSubGroup)
{
    double stdev1 = 0.189605;
    double stdev2 = 0.05455;
    int sample_size = CusumTask.getSampleSize();
    int subgroup_size = CusumTask.getSubGroupSize();
    vector<double> tmp;
    for (int index = 0; index < sample_size; index += subgroup_size)
    {
        for (int j = 0; j < subgroup_size; j++)
        {
            if (index + j < sample_size)
            {
                tmp.push_back(cusum_data[index + j]);
            }
        }
        ASSERT_NEAR(stdev1, CusumTask.calcDeviationOfSubGroup(tmp), error_margin) << "STDEV: " << stdev1;
        stdev1 = stdev2;
        tmp.clear();
    }
}

TEST(CusumTest, MultipleMeans)
{
    vector<double> stdevs = {0.189605, 0.05455};
    double mean = (0.189605 + 0.05455) / 2;
    map<string, double> tmp = {{"src_ip", mean}, {"dst_ip", mean}};
    map<string, vector<double>> std_tmp = {{"src_ip", stdevs}, {"dst_ip", stdevs}};
    for (const auto &map_pair : tmp)
    {
        ASSERT_EQ(map_pair.second, CusumTask.calcMultipleMeans(std_tmp)[map_pair.first]);
    }
}

TEST(CusumTest, Sigma)
{
    vector<double> stdevs = {0.189605, 0.05455};
    double mean = (0.189605 + 0.05455) / 2;
    map<string, double> tmp = {{"src_ip", mean / CusumTask.getCorrectionFactor()}, {"dst_ip", mean / CusumTask.getCorrectionFactor()}};
    for (const auto &map_pair : tmp){
        ASSERT_EQ(map_pair.second, CusumTask.getSigmas()[map_pair.first]) << "CORRECTION FACTOR: " << CusumTask.getCorrectionFactor();
    }
}

int main(int argc, char **argv)
{
    CusumTask.setSampleSize(4);
    CusumTask.setSubGroupSize(2);
    CusumTask.setThreshold(samples);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}