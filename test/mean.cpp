#include <vector>
#include <iostream>
#include <numeric>
using namespace std;
double calcMean(vector<double> tmp_data)
{
    double sum = accumulate(tmp_data.begin(), tmp_data.end(), 0.0);
    double mean = sum / tmp_data.size();
    return mean;
}

int main(){
    vector<double> test = {1.0};
    cout << calcMean(test);
}
