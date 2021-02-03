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
    double alpha;
    double beta;
    double delta;
    double d;
    int cusum_counter = 0;
    int subgroup_size = 0;
    int sample_size = 0;
    map<string, double> total;
    map<string, double> k;
    map<string, double> h;
    map<string, double> target_values;
    map<string, double> stdevs;
    map<string, double> S_Li_prev;
    map<string, double> S_Hi_prev;
    map<string, double> S_Li;
    map<string, double> S_Hi;
    map<string, double> UCL;
    map<string, double> LCL;
    map<string, double> norms;
    void _setVMaskVar(double false_positive_rate, double false_negative_rate, double detection_rate);
    void _setTargetValues(map<string, vector<double>> data);
    void _setStandardDeviations(map<string, vector<double>> data);
    void _calcTotal(map<string,double> means);
    void _setAlpha(double false_positive_rate);
    void _setBeta(double false_negative_rate);
    void _setDelta(double detection_rate);
    void _PrintTargetValues();
    void _PrintStandardDeviations();
    void _PrintControlLimits();
    void _PrintLogCusum();
    void _PrintSampleSize();
    void _PrintSubGroupSize();
    void _PrintGrandMeans(map<string, double> data);


public:
    void setThreshold(map<string, vector<double>> entropies);
    void calc(map<string, vector<double>> data);
    bool getThresholdStatus();
    void setSubGroupSize(int size);
    void setSampleSize(int size);
    void calcPrevCusum(map<string, vector<double>> data);
    int getSubGroupSize();
    int getSampleSize();
    map<string, double> getControlLimit();
    map<string, double> getTargetValues();
    map<string, double> getStandardDeviations();
    map<string, double> getUpperCusum();
    map<string, double> getLowerCusum();
    map<string, double> getUCL();
    map<string, double> getLCL();
    double calcMean(vector<double> tmp_data);
    double calcDeviation(vector<double> subgroup);
    map<string, double> calcNormalizedObservations(map<string, double> subgroup_mean);
    map<string, double> calcMultipleMeans(map<string, vector<double>> subgroups);
    map<string, double> calcLowerSum(map<string, double> z);
    map<string, double> calcHigherSum(map<string, double> z);
};

#endif
