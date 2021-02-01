#include <gtest/gtest.h>
#include "entropy.h"
double error_margin = 0.000001;
vector<double> Probabilities = {(double)2 / 5, (double)1 / 5, (double)2 / 5};
double normalized_entropy = 0.96023;
map<uint, uint> header_counters = {{123, 2}, {1234, 1}, {12345, 2}};
map<string, double> header_entropies = {{"src_ip", normalized_entropy}, {"dst_ip", normalized_entropy}};
map<string, map<uint, uint>> header_maps = {{"src_ip", header_counters}, {"dst_ip", header_counters}};

EntropyCalc EntropyTask;
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

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}