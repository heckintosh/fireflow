#ifndef DETECTION_HEADER
#define DETECTION_HEADER
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <math.h>
using namespace std;
double targetValue(vector<double> subgroups_means);
double sampleStdev(vector<double> tmp_data);
double calcMean(vector<double> tmp_data);
#endif