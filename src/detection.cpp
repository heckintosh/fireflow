#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <numeric>
#include <math.h>

using namespace std;

double calcMean(vector<double> tmp_data)
{
    double sum = accumulate(tmp_data.begin(), tmp_data.end(), 0.0);
    double mean = sum / tmp_data.size();
    return mean;
}

double targetValue(vector<double> entropies, double subwindow, double window){
    double size = entropies.size();
    uint total_entropies_one_window = window / subwindow; 
    uint index = 0;
    double tmp_total = 0;
    vector<double> windows_means;
    vector<double> subwindow_elems;
    for (int index = 0; index < size; ++index){
        if (index % total_entropies_one_window != 0 && index == 0){
            subwindow_elems.push_back(entropies[index]);
        }
        else{
            windows_means.push_back(calcMean(subwindow_elems));
            subwindow_elems.clear();
        }
    }
    double value = calcMean(windows_means);
    return value;
}

double sampleStdev(vector<double> tmp_data)
{
    double mean = calcMean(tmp_data);
    vector<double> diff(tmp_data.size());
    transform(tmp_data.begin(), tmp_data.end(), diff.begin(), [mean](double x) { return x - mean; });
    double sq_sum = inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = sqrt(sq_sum / (tmp_data.size() - 1));
    return stdev;
}
