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

double calcDeviationofGroup(vector<double> subgroup)
{
    double mean = calcMean(subgroup);
    double stdDev = 0;
    for (const auto &member : subgroup ){
        cout << "MEMBER OF SUBGROUP " << member << endl; 
        stdDev += pow((member - mean), 2);
    }    
    stdDev = sqrt(stdDev / (subgroup.size() - 1));
    cout << "STANDARD DEV: " << stdDev << endl;
    return stdDev;
}

double correctionFactor(int n)
{
    return sqrt(2 / (n - 1)) * (tgamma((double)n / 2) / tgamma((double)(n - 1) / 2));
}

void estimateSigma(map<string, vector<double>> samples)
{
    int subgroup_size = Capture::window / Capture::subwindow;
    double correction_factor = correctionFactor(subgroup_size);
    map<string, vector<double>> standardDeviationOfGroup;
    map<string, double> grand_standard_deviation_mean;
    map<string, double> estimation;
    vector<double> tmp;
    for (const auto &map_pair : samples)
    {
        for (int index = 0; index < map_pair.second.size(); index += subgroup_size)
        {
            for (int j = 0; j < subgroup_size; j++)
            {
                if (index + j < map_pair.second.size())
                {
                    tmp.push_back(map_pair.second[index]);
                }
            }
            standardDeviationOfGroup[map_pair.first].push_back(calcDeviationofGroup(tmp));
            tmp.clear();
        }
        cout << endl;
    }
    for (const auto &map_pair : standardDeviationOfGroup)
    {
        grand_standard_deviation_mean[map_pair.first] = calcMean(map_pair.second);
    }
    for (const auto &map_pair : grand_standard_deviation_mean)
    {
        estimation[map_pair.first] = map_pair.second / correction_factor;
    }
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
