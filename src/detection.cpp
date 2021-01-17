#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <numeric>
#include <math.h>

using namespace std;

double calcMean(vector<double> tmp_data)
{
    double sum = accumulate(tmp_data.begin(), tmp_data.end(), 0.0);
    double mean = sum / tmp_data.size();
    return mean;
}

double standardDeviation(vector<double> tmp_data, double mean)
{
    std::vector<double> diff(tmp_data.size());
    std::transform(tmp_data.begin(), tmp_data.end(), diff.begin(), [mean](double x) { return x - mean; });
    double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / tmp_data.size());
    return stdev;
}

//given a vector of the average entropies in multiple windows
// output the grand mean of those as target Value for CUSUM
double targetValue(vector<double> means_window){
    return calcMean(means_window);
}