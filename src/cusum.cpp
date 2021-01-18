#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <map>
#include <math.h>
#include "capture.h"

using namespace std;

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

void printTargetValue(map<string, double> targetvalues){
    for (const auto &targetvalue : targetvalues){
        cout << targetvalue.first << ": " << targetvalue.second << endl;
    }
}
