#ifndef CUSUM_HEADER
#define CUSUM_HEADER
#include <vector>
#include <map>
#include <string>
using namespace std;
class Cusum
{
private:
    bool thresholdStatus = false;
    double correction_factor;
    int subgroup_size = 0;
    int sample_size = 0;
    map<string, double> allowed_slacks;
    map<string, double> target_values;
    map<string, double> sigmas;
    map<string, vector<double>> samples;
    map<string, double> S_Li_prev;
    map<string, double> S_Hi_prev;
    map<string, double> S_Li;
    map<string, double> S_Hi;
    map<string, double> UCL;
    map<string, double> LCL;
    map<string, double> norms;

public:
    void setThreshold(map<string, vector<double>> entropies);
    void calc(map<string, vector<double>> data);
    bool getThresholdStatus();
    void PrintCollectSamples();
    void PrintTargetValues();
    void PrintSigmas();
    void PrintCusum();
    void PrintUCL();
    void PrintLCL();
    void setSubGroupSize(int size);
    void setSampleSize(int size);
    void _collectSamples(map<string, vector<double>> data);
    void calcPrevCusum(map<string, vector<double>> data);
    void setCorrectionFactor(int n);
    void setAllowedSlacks();
    void setUCL(map<string, double> data);
    void setLCL(map<string, double> data);
    void setTargetValues(map<string, vector<double>> data);
    void setSigma(map<string, vector<double>> data);
    int getSubGroupSize();
    int getSampleSize();
    double getCorrectionFactor();
    map<string, double> getTargetValues();
    map<string, double> getSigmas();
    map<string, double> getUpperCusum();
    map<string, double> getLowerCusum();
    map<string, double> getUCL();
    map<string, double> getLCL();
    double calcMean(vector<double> tmp_data);
    double calcDeviationOfSubGroup(vector<double> subgroup);
    map<string, double> calcNormalizedObservations(map<string, double> subgroup_mean);
    map<string, double> calcMultipleMeans(map<string, vector<double>> subgroups);
    map<string, double> calcLowerSum(map<string, double> z);
    map<string, double> calcHigherSum(map<string, double> z);
};
#endif