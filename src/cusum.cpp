#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <map>
#include <math.h>
#include "capture.h"
#include "cusum.h"

using namespace std;

double calcSampleStdev(vector<double> subgroup)
{
    double mean = calcMean(subgroup);
    vector<double> diff(subgroup.size());
    transform(subgroup.begin(), subgroup.end(), diff.begin(), [mean](double x) { return x - mean; });
    double sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = sqrt(sq_sum / (subgroup.size() - 1));
    return stdev;
}

double correctionFactor(int n)
{
    return sqrt(2 / (n - 1)) * (tgamma((double)n / 2) / tgamma((double)(n - 1) / 2));
}

void estimateSigma(map<string, vector<double>> samples)
{
    int subgroup_size = Capture::window / Capture::subwindow;
    double correction_factor = correctionFactor(subgroup_size);
    map<string, vector<double>> stdevmeans;
    map<string, double> grandstddevmean;
    map<string, double> estimation;
    vector<double> tmp;
    for (const auto &map_pair : samples)
    {
        cout << "MAP_PAIR_FIRST: " << map_pair.first << endl;
        for (int index = 0; index < map_pair.second.size(); ++index)
        {
            if (index % subgroup_size != 0 || index == 0)
            {
                cout << "MAP_PAIR: " << map_pair.second[index] << endl;
                tmp.push_back(map_pair.second[index]);
            }
            else
            {
                stdevmeans[map_pair.first].push_back(calcSampleStdev(tmp));
                tmp.clear();
            }
        }
    }
    for (const auto &map_pair : stdevmeans)
    {
        grandstddevmean[map_pair.first] = calcMean(map_pair.second);
    }
    for (const auto &map_pair : grandstddevmean)
    {
        estimation[map_pair.first] = map_pair.second / correction_factor;
    }
    printMapStringDouble(estimation);
}

double calcMean(vector<double> tmp_data)
{
    double sum = accumulate(tmp_data.begin(), tmp_data.end(), 0.0);
    double mean = sum / tmp_data.size();
    return mean;
}

map<string, double> targetValue(map<string, vector<double>> samples)
{
    map<string, double> tmp;
    for (const auto &map_pair : samples)
    {
        tmp[map_pair.first] = calcMean(map_pair.second);
    }
    return tmp;
}

void printMapStringDouble(map<string, double> targetvalues)
{
    for (const auto &targetvalue : targetvalues)
    {
        cout << targetvalue.first << ": " << targetvalue.second << endl;
    }
}
