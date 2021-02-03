#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include "cusum.h"
#include "capture.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
void Cusum::setThreshold(map<string, vector<double>> samples)
{
    if (Cusum::thresholdStatus == false)
    {
        Cusum::thresholdStatus = true;
        _setTargetValues(samples);
        _setStandardDeviations(samples);
        _setVMaskVar(0.0027, 0.0001, 1);
        for (const auto &map_pair : samples)
        {
            S_Li_prev[map_pair.first] = 0.0;
            S_Hi_prev[map_pair.first] = 0.0;
            Cusum::total[map_pair.first] = 0.0;
        }
        Cusum::calcPrevCusum(samples);
    }
    else if (Cusum::thresholdStatus == true)
    {
        cout << "The Cusum Threshold has already been set. Check for errors in code." << endl;
        exit(1);
    }
}

void Cusum::_setVMaskVar(double false_positive_rate, double false_negative_rate, double detection_rate)
{
    _setAlpha(false_positive_rate);
    _setBeta(false_negative_rate);
    _setDelta(detection_rate);
    for (const auto &map_pair : Cusum::stdevs)
    {
        Cusum::k[map_pair.first] = (map_pair.second * delta) / 2;
    }
    Cusum::d = 2 / (pow(Cusum::delta, 2)) * log((1 - Cusum::beta) / (Cusum::alpha));
    for (const auto &map_pair : Cusum::k)
    {
        Cusum::h[map_pair.first] = map_pair.second * Cusum::d;
    }
    _PrintControlLimits();
}

void Cusum::_setStandardDeviations(map<string, vector<double>> data)
{
    map<string, double> standardDeviations;
    //fill and calculate standard deviations all windows in data
    for (const auto &map_pair : data)
    {
        Cusum::stdevs[map_pair.first] = calcDeviation(map_pair.second) / pow(Cusum::subgroup_size, 0.5);
    }
    _PrintStandardDeviations();
}

// Calculate and return Cusum values for detection and plotting
// data: either samples of multiple subgroups for  _setting threshold or data of a subgroup for performing detection
void Cusum::calc(map<string, vector<double>> data)
{
    map<string, double> means = calcMultipleMeans(data);
    Cusum::cusum_counter += 1;
    Cusum::S_Li = calcLowerSum(means);
    Cusum::S_Hi = calcHigherSum(means);
    _calcTotal(means);
    _PrintLogCusum();
    Cusum::S_Li_prev = Cusum::S_Li;
    Cusum::S_Hi_prev = Cusum::S_Hi;
}

void Cusum::_calcTotal(map<string,double> means){
    for (auto &map_pair : Cusum::total){
        map_pair.second += means[map_pair.first] - target_values[map_pair.first];
    }
}

void Cusum::calcPrevCusum(map<string, vector<double>> data)
{
    map<string, vector<double>> dataPiece;
    for (int j = 0; j < sample_size; j += subgroup_size)
    {
        //Take data samples from one window and calculate CUSUM
        for (int i = 0; i < subgroup_size; ++i)
        {
            for (const auto &map_pair : data)
            {
                if (i + j < sample_size)
                {
                    dataPiece[map_pair.first].push_back(map_pair.second[i + j]);
                }
            }
        }
        Cusum::calc(dataPiece);
        for (auto &map_pair : dataPiece)
        {
            map_pair.second.clear();
        }
    }
}

void Cusum::_setTargetValues(map<string, vector<double>> data)
{
    for (const auto &map_pair : data)
    {
        target_values[map_pair.first] = calcMean(map_pair.second);
    }
    _PrintTargetValues();
}

//calculate entropies deviation of each windows
double Cusum::calcDeviation(vector<double> subgroup)
{
    double mean = calcMean(subgroup);
    double stdDev = 0;
    for (const auto &member : subgroup)
    {
        stdDev += pow((member - mean), 2);
    }
    stdDev = sqrt(stdDev / (subgroup.size()));
    return stdDev;
}

double Cusum::calcMean(vector<double> tmp_data)
{
    double sum = accumulate(tmp_data.begin(), tmp_data.end(), 0.0);
    double mean = sum / tmp_data.size();
    return mean;
}

// calculate upper cummulative sum
// z is the normalized observations
map<string, double> Cusum::calcHigherSum(map<string, double> z)
{
    map<string, double> tmp;
    for (const auto &map_pair : z)
    {
        tmp[map_pair.first] = max(0.0, (map_pair.second + S_Hi_prev[map_pair.first] - target_values[map_pair.first] - k[map_pair.first]));
    }
    return tmp;
}

// calculate lower cummulative sum
// z is the normalized observations, k is allowable slack: typically _set to 0.5 sigma
map<string, double> Cusum::calcLowerSum(map<string, double> z)
{
    map<string, double> tmp;
    for (const auto &map_pair : z)
    {
        tmp[map_pair.first] = max(0.0, (S_Li_prev[map_pair.first] - map_pair.second + target_values[map_pair.first] - k[map_pair.first]));
    }
    return tmp;
}

bool Cusum::getThresholdStatus()
{
    return Cusum::thresholdStatus;
}

// calculate means of data collected in one window
map<string, double> Cusum::calcMultipleMeans(map<string, vector<double>> data)
{
    map<string, double> means;
    for (const auto &map_pair : data)
    {
        means[map_pair.first] = calcMean(map_pair.second);
    }
    return means;
}

map<string, double> Cusum::getControlLimit()
{
    return Cusum::h;
}

map<string, double> Cusum::getUpperCusum()
{
    return Cusum::S_Hi;
}

map<string, double> Cusum::getLowerCusum()
{
    return Cusum::S_Li;
}

map<string, double> Cusum::getStandardDeviations()
{
    return Cusum::stdevs;
}

map<string, double> Cusum::getTargetValues()
{
    return Cusum::target_values;
}

void Cusum::setSubGroupSize(int data)
{
    Cusum::subgroup_size = data;
    Cusum::_PrintSubGroupSize();
}

void Cusum::setSampleSize(int data)
{
    Cusum::sample_size = data;
    Cusum::_PrintSampleSize();
}

int Cusum::getSubGroupSize()
{
    return Cusum::subgroup_size;
}

int Cusum::getSampleSize()
{
    return Cusum::sample_size;
}

void Cusum::_setAlpha(double false_positive_rate)
{
    Cusum::alpha = false_positive_rate;
}

void Cusum::_setBeta(double false_negative_rate)
{
    Cusum::beta = false_negative_rate;
}

void Cusum::_setDelta(double detection_rate)
{
    Cusum::delta = detection_rate;
}

void Cusum::_PrintLogCusum()
{
    cout << "------------------------------------------------" << endl;

    cout << "LOWER SUM: " << endl;
    for (const auto &map_pair : S_Li)
    {
        cout << map_pair.first << ": " << map_pair.second << " ";
        spdlog::get("cusum_logger")->info("{} {} {} {} {}", map_pair.first, Cusum::cusum_counter, map_pair.second, Cusum::S_Hi[map_pair.first], Cusum::total[map_pair.first]);
    }
    cout << endl;

    cout << "HIGHER SUM: " << endl;
    for (const auto &map_pair : S_Hi)
    {
        cout << map_pair.first << ": " << map_pair.second << " ";
    }
    cout << endl;

    cout << "LOWER SUM PREV: " << endl;
    for (const auto &map_pair : S_Li_prev)
    {
        cout << map_pair.first << ": " << map_pair.second << " ";
    }
    cout << endl;

    cout << "HIGHER SUM PREV: " << endl;
    for (const auto &map_pair : S_Hi_prev)
    {
        cout << map_pair.first << ": " << map_pair.second << " ";
    }
    cout << endl;
    cout << endl;
    cout << "------------------------------------------------" << endl;
}

void Cusum::_PrintStandardDeviations()
{
    cout << "---------STANDARD DEVIATIONS----------: " << endl;
    for (const auto &map_pair : Cusum::getStandardDeviations())
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

void Cusum::_PrintTargetValues()
{
    cout << "-------TARGET VALUES--------: " << endl;
    for (const auto &map_pair : Cusum::getTargetValues())
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

void Cusum::_PrintSampleSize()
{
    cout << "-------SAMPLE SIZE----------: " << endl;
    cout << Cusum::sample_size << endl;
}

void Cusum::_PrintSubGroupSize()
{
    cout << "-------SUBGROUP SIZE---------" << endl;
    cout << Cusum::subgroup_size << endl;
}

void Cusum::_PrintGrandMeans(map<string, double> data)
{
    cout << "-------GRAND MEANS---------" << endl;
    for (const auto &map_pair : data)
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

void Cusum::_PrintControlLimits()
{
    cout << "-------CONTROL LIMITS---------" << endl;
    for (const auto &map_pair : h)
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}