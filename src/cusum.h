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
    map<string, double> norms;
    double correction_factor;
    int subgroup_size = Capture::window / Capture::subwindow;
    int sample_size = subgroup_size * Capture::estimator;
    void _collectSamples(map<string, vector<double>> data);
    void _calcPrevThreshold(map<string, vector<double>> data);
    map<string, double> _calcNormalizedObservations(map<string, double> subgroup_mean);
    map<string, double> _calcMultipleMeans(map<string, vector<double>> subgroups);
    map<string, double> _calcLowerSum(map<string, double> z);
    map<string, double> _calcHigherSum(map<string, double> z);
    map<string, double> _setAllowedSlacks();
    double _calcMean(vector<double> tmp_data);

public:
    double setCorrectionFactor(int n);
    double calcDeviationOfSubGroup(vector<double> subgroup);
    map<string, double> estimateSigma(map<string, vector<double>> data);
    map<string, double> setTargetValues(map<string, vector<double>> data);
    void setThreshold(map<string, vector<double>> entropies);
    void calc(map<string, vector<double>> data);
    bool getThresholdStatus();
    void PrintCollectSamples();
    void PrintTargetValues();
    void PrintSigmas();
    void PrintCuSum();
};
#endif