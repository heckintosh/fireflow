#include <iostream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include "capture.h"
#include "cusum.h"

using namespace std;

// Calculate and return Cusum values for detection and plotting
// data: either samples of multiple subgroups for  setting threshold or data of a subgroup for performing detection
void Cusum::calc(map<string, vector<double>> data)
{
    map<string, double> means = _calcMultipleMeans(data);
    map<string, double> norms = _calcNormalizedObservations(means);
    Cusum::S_Li = _calcLowerSum(norms);
    Cusum::S_Hi = _calcHigherSum(norms);
    Cusum::S_Li_prev = Cusum::S_Li;
    Cusum::S_Hi_prev = Cusum::S_Hi;
}

void Cusum::_calcPrevCusum(map<string, vector<double>> data)
{
    map<string, vector<double>> dataPiece;
    for (int j = 0; j < sample_size; j += subgroup_size)
    {
        //Take data samples from one window and calculate CUSUM 
        for (int i = 0; i < subgroup_size; ++i)
        {
            for (const auto &map_pair : data)
            {
                if (i + j < sample_size){
                    dataPiece[map_pair.first].push_back(map_pair.second[i + j]);
                }
            }
        }
        Cusum::calc(dataPiece);
        for (auto &map_pair : dataPiece){
            map_pair.second.clear();
        }
    }
}

// calculate means of data collected in one window
map<string, double> Cusum::_calcMultipleMeans(map<string, vector<double>> data)
{
    map<string, double> means;
    for (const auto &map_pair : data)
    {
        means[map_pair.first] = _calcMean(map_pair.second);
    }
    return means;
}

void Cusum::setThreshold(map<string, vector<double>> samples)
{
    if (Cusum::thresholdStatus == false)
    {
        _collectSamples(samples);
        Cusum::thresholdStatus = true;
        _setCorrectionFactor(subgroup_size);
        _setTargetValues(samples);
        _setSigma(samples);
        _setAllowedSlacks();
        _setUCL(Cusum::sigmas);
        _setLCL(Cusum::sigmas);
        for (const auto &map_pair : samples)
        {
            S_Li_prev[map_pair.first] = 0.0;
            S_Hi_prev[map_pair.first] = 0.0;
        }
        Cusum::_calcPrevCusum(samples);
    }
    else if (Cusum::thresholdStatus == true)
    {
        cout << "The Cusum Threshold has already been set. Check for errors in code." << endl;
        exit(1);
    }
}

// calculate lower cummulative sum
// z is the normalized observations, k is allowable slack: typically set to 0.5 sigma
map<string, double> Cusum::_calcLowerSum(map<string, double> z)
{
    map<string, double> tmp;
    for (const auto &map_pair : z)
    {
        tmp[map_pair.first] = -max(0.0, (-map_pair.second - Cusum::allowed_slacks[map_pair.first] + S_Li_prev[map_pair.first]));
    }
    return tmp;
}

//After setting threshold, we get target_values as grand means and sigmas
map<string, double> Cusum::_calcNormalizedObservations(map<string, double> subgroup_mean)
{
    map<string, double> z;
    for (const auto &map_pair : subgroup_mean)
    {
        z[map_pair.first] = (map_pair.second - target_values[map_pair.first]) / sigmas[map_pair.first];
    }
    return z;
}

void Cusum::PrintCollectSamples()
{
    for (const auto &map_pair : Cusum::samples)
    {
        cout << map_pair.first << ": ";
        for (const auto &member : map_pair.second)
        {
            cout << member << " ";
        }
        cout << endl;
    }
}

void Cusum::_setTargetValues(map<string, vector<double>> data)
{
    for (const auto &map_pair : data)
    {
        target_values[map_pair.first] = _calcMean(map_pair.second);
    }
}

//calculate entropies deviation of each windows
double Cusum::_calcDeviationOfSubGroup(vector<double> subgroup)
{
    double mean = _calcMean(subgroup);
    double stdDev = 0;
    for (const auto &member : subgroup)
    {
        stdDev += pow((member - mean), 2);
    }
    stdDev = sqrt(stdDev / (subgroup.size()));
    return stdDev;
}

void Cusum::_setCorrectionFactor(int n)
{
    Cusum::correction_factor = sqrt(2 / (n - 1)) * (tgamma((double)n / 2) / tgamma((double)(n - 1) / 2));
}

double Cusum::_calcMean(vector<double> tmp_data)
{
    double sum = accumulate(tmp_data.begin(), tmp_data.end(), 0.0);
    double mean = sum / tmp_data.size();
    return mean;
}

//estimating sigma from a dict with headers as keys and multi entropies of subgroups as values
// subgroup is the amount of entropies values being calculated in one window
void Cusum::_setSigma(map<string, vector<double>> data)
{
    map<string, vector<double>> standardDeviationOfGroup;
    map<string, double> grand_standard_deviation_mean;
    vector<double> tmp;

    //fill and calculate standard deviations all windows in data
    for (const auto &map_pair : data)
    {
        for (int index = 0; index < map_pair.second.size(); index += subgroup_size)
        {
            for (int j = 0; j < subgroup_size; j++)
            {
                if (index + j < map_pair.second.size())
                {
                    tmp.push_back(map_pair.second[index + j]);
                }
            }
            standardDeviationOfGroup[map_pair.first].push_back(_calcDeviationOfSubGroup(tmp));
            tmp.clear();
        }
    }

    grand_standard_deviation_mean = _calcMultipleMeans(standardDeviationOfGroup);
    for (const auto &map_pair : grand_standard_deviation_mean)
    {
        Cusum::sigmas[map_pair.first] = map_pair.second / correction_factor;
    }
}

// calculate upper cummulative sum
// z is the normalized observations
map<string, double> Cusum::_calcHigherSum(map<string, double> z)
{
    map<string, double> tmp;
    for (const auto &map_pair : z)
    {
        tmp[map_pair.first] = max(0.0, (map_pair.second - Cusum::allowed_slacks[map_pair.first] + S_Hi_prev[map_pair.first]));
    }
    return tmp;
}

void Cusum::PrintTargetValues()
{
    for (const auto &map_pair : target_values)
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

void Cusum::PrintSigmas()
{
    for (const auto &map_pair : sigmas)
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

void Cusum::PrintCuSum()
{
    cout << "LOWER SUM: " << endl;
    for (const auto &map_pair : S_Li)
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
    cout << "HIGHER SUM: " << endl;
    for (const auto &map_pair : S_Hi)
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

//Set allowed slacks based on sigmas values:
void Cusum::_setAllowedSlacks()
{
    for (const auto &map_pair : sigmas)
    {
        Cusum::allowed_slacks[map_pair.first] = 0.5 * map_pair.second;
    }
}

void Cusum::_collectSamples(map<string, vector<double>> data)
{
    Cusum::samples = data;
}

bool Cusum::getThresholdStatus(){
    return Cusum::thresholdStatus;
}

void Cusum::_setUCL(map<string, double> data){
    for (const auto &map_pair : data){
        Cusum::UCL[map_pair.first] = 5 * map_pair.second;         
    }
}

void Cusum::_setLCL(map<string, double> data){
    for (const auto &map_pair : data){
        Cusum::LCL[map_pair.first] = -5 * map_pair.second;         
    }
}

map<string, double> Cusum::getUCL(){
    return Cusum::UCL;
}

map<string, double> Cusum::getLCL(){
    return Cusum::LCL;
}

map<string, double> Cusum::getUpperCusum(){
    return Cusum::S_Hi;
}

map<string, double> Cusum::getLowerCusum(){
    return Cusum::S_Li;
}