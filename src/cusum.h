#ifndef CUSUM_HEADER
#define CUSUM_HEADER
#include <vector>
#include <map>
#include <string>
using namespace std;
double calcMean(vector<double> tmp_data);
map<string, double> targetValue(map<string, vector<double>> samples);
double calcSampleStdev(vector<double> subgroup);
double correctionFactor(int n);
void estimateSigma(map<string, vector<double>> samples);
void printMapStringDouble(map<string, double> targetvalues);
#endif