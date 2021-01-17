#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <random>
using namespace std;
double calcEntropy(vector<double> probability){
    double total = 0;
    double size =probability.size();
    for (const double &p : probability)
    {
        total += -(p * log(p) / log(size));
    }
    return total;
};

vector<double> calcProb(vector<double> test)
{
    double total = test.size();
    vector<double> probs;
    vector<double> tmp;
    for (const auto &p : test)
    {
        if (!count(tmp.begin(), tmp.end(), p)){
            tmp.push_back(p);
            probs.push_back(count(test.begin(), test.end(), p) / total);
        }        
    }
    return probs;
};

int main(){
    vector<double> test = {1,1,1,2};
    vector<double> prob = {0.75};
    cout << calcEntropy(prob);
}