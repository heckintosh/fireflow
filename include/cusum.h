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
    double correction_factor;
    int subgroup_size = Capture::window / Capture::subwindow;
    int sample_size = subgroup_size * Capture::estimator;
    void _collectSamples(map<string, vector<double>> data);
    void _calcPrevCusum(map<string, vector<double>> data);
    map<string, double> _calcNormalizedObservations(map<string, double> subgroup_mean);
    map<string, double> _calcMultipleMeans(map<string, vector<double>> subgroups);
    map<string, double> _calcLowerSum(map<string, double> z);
    map<string, double> _calcHigherSum(map<string, double> z);
    void _setCorrectionFactor(int n);
    void _setAllowedSlacks();
    void _setUCL(map<string, double> data);
    void _setLCL(map<string, double> data);
    void _setTargetValues(map<string, vector<double>> data);
    void _setSigma(map<string, vector<double>> data);
    double _calcMean(vector<double> tmp_data);
    double _calcDeviationOfSubGroup(vector<double> subgroup);


public:
    void setThreshold(map<string, vector<double>> entropies);
    void calc(map<string, vector<double>> data);
    bool getThresholdStatus();
    void PrintCollectSamples();
    void PrintTargetValues();
    void PrintSigmas();
    void PrintCuSum();
    map<string, double> getUpperCusum();
    map<string, double> getLowerCusum();
    map<string, double> getUCL();
    map<string, double> getLCL();


};
#endif