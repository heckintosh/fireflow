#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <math.h>
#include "cusum.h"
#include "capture.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
using namespace tabulate;
using namespace std;
using Row_t = std::vector<variant<std::string, const char *, Table>>;

//Take in a map of string label with samples data, set threshold using those data
void Cusum::setThreshold(int subsize, int samplesize, map<string, vector<double>> samples)
{
    Cusum::cusumTable = generateCusumTable(Capture::attributes);
    // If not initialized then initialized
    if (Cusum::subgroup_size == 0 || Cusum::sample_size == 0)
    {
        Cusum::sample_size = samplesize;
        Cusum::subgroup_size = subsize;
    }
    if (Cusum::thresholdStatus == false)
    {
        Cusum::thresholdStatus = true;
        // Set Target Values based on the samples from beginning to the time of reaching threshold value
        _setTargetValues(samples);
        // Set Tabular variables: (Correaction Factor + Standard deviation + k + h)
        _setTabularVar(samples);
        // Initialize some essential values to 0
        spdlog::get("exec_logger")->info("Threshold is going to be set now.");
        spdlog::get("exec_logger")->info("SAMPLE SIZE FOR THIS THRESHOLD: {}", Cusum::sample_size);
        for (const auto &map_pair : samples)
        {
            S_Li_prev[map_pair.first] = 0.0;
            S_Hi_prev[map_pair.first] = 0.0;
            spdlog::get("exec_logger")->info("Target values of {}: {}", map_pair.first, Cusum::getControlLimit()[map_pair.first]);
        }
        for (const auto &map_pair : samples)
        {
        }
        Cusum::_calcPrevCusum(samples);
        spdlog::get("exec_logger")->info("Threshold is reached and set.");
    }
    else if (Cusum::thresholdStatus == true)
    {
        cout << "The Cusum Threshold has already been set. The setThreshold function should not be called if its already set." << endl;
        exit(1);
    }
    Cusum::_logCusum(Cusum::cusumTable, "threshold_logger");
}

// Loop through each samples before the threshold point, calculate Cusum for each windows
void Cusum::_calcPrevCusum(map<string, vector<double>> data)
{
    map<string, vector<double>> dataPiece;
    spdlog::get("exec_logger")->info("SAMPLE SIZE FOR THIS THRESHOLD: {}", Cusum::sample_size);
    for (int j = 0; j < Cusum::sample_size; j += Cusum::subgroup_size)
    {
        //Take data samples from one window and put into a tmp map
        for (int i = 0; i < subgroup_size; ++i)
        {
            for (const auto &map_pair : data)
            {
                if (i + j < Cusum::sample_size)
                {
                    dataPiece[map_pair.first].push_back(map_pair.second[i + j]);
                }
            }
        }
        //Calculate CUSUM on that map
        Cusum::calc(dataPiece);
        //Logging the threshold Cusum table:

        //Clear map for the next loop
        for (auto &map_pair : dataPiece)
        {
            map_pair.second.clear();
        }
    }
}

// Calculate and return Cusum values for detection and plotting
// data: either samples of multiple subgroups for  setting threshold or data of a subgroup for performing detection
// Also add some additional logging
void Cusum::calc(map<string, vector<double>> data)
{
    map<string, double> means = calcMultipleMeans(data);
    Cusum::S_Li = _calcLowerSum(means);
    Cusum::S_Hi = _calcHigherSum(means);
    Cusum::subgroup_counter += 1;
    Cusum::_calcTotal(means);
    Cusum::S_Li_prev = Cusum::S_Li;
    Cusum::S_Hi_prev = Cusum::S_Hi;
    updateCusumTable(Cusum::cusumTable, Cusum::subgroup_counter, Cusum::S_Li, Cusum::S_Hi);
    if (Cusum::thresholdStatus == true)
    {
        Cusum::_logCusum(Cusum::cusumTable, "cusum_logger");
    }
}

void Cusum::_setStandardDeviationsTabular(map<string, vector<double>> data)
{
    map<string, vector<double>> standardDeviationOfGroup;
    map<string, double> grand_standard_deviation_mean;
    vector<double> tmp;

    //fill and calculate standard deviations all windows in data
    for (const auto &map_pair : data)
    {
        for (int index = 0; index < sample_size; index += subgroup_size)
        {
            for (int j = 0; j < subgroup_size; j++)
            {
                if (index + j < sample_size)
                {
                    tmp.push_back(map_pair.second[index + j]);
                }
            }
            standardDeviationOfGroup[map_pair.first].push_back(_calcDeviation(tmp));
            tmp.clear();
        }
    }

    grand_standard_deviation_mean = calcMultipleMeans(standardDeviationOfGroup);
    for (const auto &map_pair : grand_standard_deviation_mean)
    {
        Cusum::stdevs[map_pair.first] = map_pair.second / correction_factor;
    }
}

void Cusum::_setCorrectionFactor(int n)
{
    double tmp = n;
    Cusum::correction_factor = sqrt(2 / (tmp - 1)) * (tgamma((double)tmp / 2) / tgamma((double)(tmp - 1) / 2));
}

void Cusum::_calcTotal(map<string, double> means)
{
    for (auto &map_pair : Cusum::total)
    {
        map_pair.second += means[map_pair.first] - target_values[map_pair.first];
    }
}

void Cusum::_setTargetValues(map<string, vector<double>> data)
{
    for (const auto &map_pair : data)
    {
        target_values[map_pair.first] = calcMean(map_pair.second);
    }
}

//calculate entropies deviation of each windows
double Cusum::_calcDeviation(vector<double> subgroup)
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
map<string, double> Cusum::_calcHigherSum(map<string, double> z)
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
map<string, double> Cusum::_calcLowerSum(map<string, double> z)
{
    map<string, double> tmp;
    for (const auto &map_pair : z)
    {
        tmp[map_pair.first] = min(0.0, (S_Li_prev[map_pair.first] + map_pair.second - target_values[map_pair.first] + k[map_pair.first]));
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

void Cusum::_setTabularVar(map<string, vector<double>> data)
{
    _setCorrectionFactor(Cusum::subgroup_size);
    _setStandardDeviationsTabular(data);
    for (const auto &map_pair : Cusum::stdevs)
    {
        Cusum::k[map_pair.first] = map_pair.second / 2;
        Cusum::h[map_pair.first] = map_pair.second * 4.77;
    }
}

map<string, double> Cusum::getControlLimit()
{
    return Cusum::h;
}

map<string, double> Cusum::getTargetValues()
{
    return Cusum::target_values;
}

int Cusum::getSubCount()
{
    return Cusum::subgroup_counter;
}

map<string, double> Cusum::getUpperCusum()
{
    return Cusum::S_Hi;
}

map<string, double> Cusum::getLowerCusum()
{
    return Cusum::S_Li;
}

map<string, Table> Cusum::generateCusumTable(vector<string> tableNames)
{
    map<string, Table> titleTables;
    map<string, Table> cusumTables;
    for (int i = 0; i < tableNames.size(); i++)
    {
        titleTables[tableNames[i]].add_row(Row_t{tableNames[i]});
        cusumTables[tableNames[i]].add_row(Row_t{titleTables[tableNames[i]]});
    }
    return cusumTables;
}

void Cusum::updateCusumTable(map<string, Table> &cusumTable, int index, map<string, double> S_Li, map<string, double> S_Hi)
{
    for (auto &map_pair : cusumTable)
    {
        Table tmp;
        tmp.add_row(Row_t{to_string(index), to_string(S_Li[map_pair.first]), to_string(S_Hi[map_pair.first])});
        map_pair.second.add_row(Row_t{tmp});
    }
}

// Function for logging sum values
void Cusum::_logCusum(map<string, Table> cusumTable, string loggername)
{
    std::stringstream ss;
    //Only loop through the lowerSum keys, because both have the same keys
    for (const auto &map_pair : cusumTable)
    {
        ss << map_pair.second;
        spdlog::get(loggername)->info("{}", ss.str());
        ss.str("");
    }
}
