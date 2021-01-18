#ifndef CUSUM_HEADER
#define CUSUM_HEADER
#include <vector>
#include <map>
#include <string>
using namespace std;
double calcMean(vector<double> tmp_data);
map<string, double> targetValue(map<string, vector<double>> samples);
void printTargetValue(map<string, double> targetvalues);
#endif