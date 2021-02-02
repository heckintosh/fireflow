#include <iostream>
#include <algorithm>
#include <numeric>
#include <math.h>
#include "capture.h"
#include "cusum.h"

using namespace std;

void Cusum::setThreshold(map<string, vector<double>> samples)
{
    if (Cusum::thresholdStatus == false)
    {
        Cusum::thresholdStatus = true;
        setCorrectionFactor(subgroup_size);
        setTargetValues(samples);
        setSigma(samples);
        setAllowedSlacks();
        setUCL(Cusum::sigmas);
        setLCL(Cusum::sigmas);
        for (const auto &map_pair : samples)
        {
            S_Li_prev[map_pair.first] = 0.0;
            S_Hi_prev[map_pair.first] = 0.0;
        }
        Cusum::calcPrevCusum(samples);
    }
    else if (Cusum::thresholdStatus == true)
    {
        cout << "The Cusum Threshold has already been set. Check for errors in code." << endl;
        exit(1);
    }
}

//estimating sigma from a dict with headers as keys and multi entropies of subgroups as values
// subgroup is the amount of entropies values being calculated in one window
void Cusum::setSigma(map<string, vector<double>> data)
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
            standardDeviationOfGroup[map_pair.first].push_back(calcDeviationOfSubGroup(tmp));
            tmp.clear();
        }
    }

    grand_standard_deviation_mean = calcMultipleMeans(standardDeviationOfGroup);
    PrintGrandMeans(grand_standard_deviation_mean);
    for (const auto &map_pair : grand_standard_deviation_mean)
    {
        Cusum::sigmas[map_pair.first] = map_pair.second / correction_factor;
    }
    PrintSigmas();
}

// Calculate and return Cusum values for detection and plotting
// data: either samples of multiple subgroups for  setting threshold or data of a subgroup for performing detection
void Cusum::calc(map<string, vector<double>> data)
{
    map<string, double> means = calcMultipleMeans(data);
    map<string, double> norms = calcNormalizedObservations(means);
    Cusum::S_Li = calcLowerSum(norms);
    Cusum::S_Hi = calcHigherSum(norms);
    Cusum::S_Li_prev = Cusum::S_Li;
    Cusum::S_Hi_prev = Cusum::S_Hi;
    PrintCusum();
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

// calculate lower cummulative sum
// z is the normalized observations, k is allowable slack: typically set to 0.5 sigma
map<string, double> Cusum::calcLowerSum(map<string, double> z)
{
    map<string, double> tmp;
    for (const auto &map_pair : z)
    {
        tmp[map_pair.first] = -max(0.0, (-map_pair.second - Cusum::allowed_slacks[map_pair.first] + S_Li_prev[map_pair.first]));
    }
    return tmp;
}

map<string, double> Cusum::calcNormalizedObservations(map<string, double> subgroup_mean)
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

void Cusum::setTargetValues(map<string, vector<double>> data)
{
    for (const auto &map_pair : data)
    {
        target_values[map_pair.first] = calcMean(map_pair.second);
    }
    PrintTargetValues();
}

//calculate entropies deviation of each windows
double Cusum::calcDeviationOfSubGroup(vector<double> subgroup)
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

void Cusum::setCorrectionFactor(int n)
{
    double tmp = n;
    if (tmp != 0 && tmp != 1)
    {
        Cusum::correction_factor = sqrt(2 / (tmp - 1)) * (tgamma((double)tmp / 2) / tgamma((double)(tmp - 1) / 2));
    }
    else
    {
        Cusum::correction_factor = 1;
    }
    Cusum::PrintCorrectionFactor();
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
        tmp[map_pair.first] = max(0.0, (map_pair.second - Cusum::allowed_slacks[map_pair.first] + S_Hi_prev[map_pair.first]));
    }
    return tmp;
}

void Cusum::PrintCusum()
{
    cout << "------------------------------------------------" << endl;
    
    cout << "LOWER SUM: " << endl;
    for (const auto &map_pair : S_Li)
    {
        cout << map_pair.first << ": " << map_pair.second << " ";
    }
    cout << endl;
    cout << endl
         << "HIGHER SUM: " << endl;
    for (const auto &map_pair : S_Hi)
    {
        cout << map_pair.first << ": " << map_pair.second << " ";
    }
    cout << endl;
        cout << "HIGHER SUM PREV: " << endl;
    for (const auto &map_pair : S_Li_prev)
    {
        cout << map_pair.first << ": " << map_pair.second << " ";
    }
    cout << endl; 
    cout << "------------------------------------------------" << endl;
}

void Cusum::PrintUCL()
{
    for (const auto &map_pair : Cusum::getUCL())
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

void Cusum::PrintCorrectionFactor()
{
    cout << "-------CORRECTION FACTOR--------: " << endl;
    cout << Cusum::correction_factor << endl;
}

void Cusum::PrintSigmas()
{
    cout << "---------SIGMAS----------: " << endl;
    for (const auto &map_pair : Cusum::getSigmas())
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

void Cusum::PrintTargetValues()
{
    cout << "-------TARGET VALUES--------: " << endl;
    for (const auto &map_pair : Cusum::getTargetValues())
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

void Cusum::PrintSampleSize()
{
    cout << "-------SAMPLE SIZE----------: " << endl;
    cout << Cusum::sample_size << endl;
}

void Cusum::PrintSubGroupSize()
{
    cout << "-------SUBGROUP SIZE---------" << endl;
    cout << Cusum::subgroup_size << endl;
}

void Cusum::PrintLCL()
{
    for (const auto &map_pair : Cusum::getLCL())
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

void Cusum::PrintGrandMeans(map<string, double> data)
{
    cout << "-------GRAND MEANS---------" << endl;
    for (const auto &map_pair : data)
    {
        cout << map_pair.first << ": " << map_pair.second << endl;
    }
}

//Set allowed slacks based on sigmas values:
void Cusum::setAllowedSlacks()
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

bool Cusum::getThresholdStatus()
{
    return Cusum::thresholdStatus;
}

void Cusum::setUCL(map<string, double> data)
{
    for (const auto &map_pair : data)
    {
        Cusum::UCL[map_pair.first] = 5 * map_pair.second;
    }
}

void Cusum::setLCL(map<string, double> data)
{
    for (const auto &map_pair : data)
    {
        Cusum::LCL[map_pair.first] = -5 * map_pair.second;
    }
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

map<string, double> Cusum::getUCL()
{
    return Cusum::UCL;
}

map<string, double> Cusum::getLCL()
{
    return Cusum::LCL;
}

map<string, double> Cusum::getUpperCusum()
{
    return Cusum::S_Hi;
}

map<string, double> Cusum::getLowerCusum()
{
    return Cusum::S_Li;
}

map<string, double> Cusum::getSigmas()
{
    return Cusum::sigmas;
}

map<string, double> Cusum::getTargetValues()
{
    return Cusum::target_values;
}

void Cusum::setSubGroupSize(int data)
{
    Cusum::subgroup_size = data;
    Cusum::PrintSubGroupSize();
}

void Cusum::setSampleSize(int data)
{
    Cusum::sample_size = data;
    Cusum::PrintSampleSize();
}

int Cusum::getSubGroupSize()
{
    return Cusum::subgroup_size;
}

int Cusum::getSampleSize()
{
    return Cusum::sample_size;
}

double Cusum::getCorrectionFactor()
{
    return Cusum::correction_factor;
}
