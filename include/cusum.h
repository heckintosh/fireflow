#ifndef CUSUM_HEADER
#define CUSUM_HEADER
#include <vector>
#include <map>
#include <string>
#include "tabulate.hpp"
using namespace std;
using namespace tabulate;
class Cusum
{
private:
    bool thresholdStatus = false;
    double alpha;
    double beta;
    double delta;
    double correction_factor;
    int subgroup_counter = 0; //Count how many subgroups have been collected and calculated CUSUM on
    int subgroup_size = 0;
    int sample_size = 0;
    map<string, Table> cusumTable;
    map<string, double> h;
    map<string, double> total;
    map<string, double> k;
    map<string, double> target_values;
    map<string, double> stdevs;
    map<string, double> S_Li_prev;
    map<string, double> S_Hi_prev;
    map<string, double> S_Li;
    map<string, double> S_Hi;
    map<string, int> ShiftH_counter;
    map<string, int> ShiftL_counter;
    double _calcDeviation(vector<double> subgroup);
    void _setTabularVar(map<string, vector<double>> data);
    void _setTargetValues(map<string, vector<double>> data);
    void _setStandardDeviationsTabular(map<string, vector<double>> data);
    void _calcTotal(map<string, double> means);
    void _setCorrectionFactor(int n);
    void _setControlLimits();
    void _shiftCounter();
    void _PrintGrandMeans(map<string, double> data);
    void _calcPrevCusum(map<string, vector<double>> data);
    void _logCusum(map<string, Table> tableObject);
    map<string, double> _calcLowerSum(map<string, double> z);
    map<string, double> _calcHigherSum(map<string, double> z);
    map<string, Table> generateCusumTable(vector<string> tableNames);
    void coutCusumTable(map<string, Table> cusumTable);
    void updateCusumTable(map<string, Table> &cusumTable, int index, map<string, double> S_Li, map<string, double> S_Hi);

public:
    void setThreshold(int subsize, int samplesize, map<string, vector<double>> entropies);
    void reset(string LiOrHi, string header2reset);
    bool getThresholdStatus();
    int getSubCount();
    map<string, int> getShiftCounterUpper();
    map<string, int> getShiftCounterLower();
    map<string, double> getControlLimit();
    map<string, double> getTargetValues();
    map<string, double> getStandardDeviations();
    map<string, double> getUpperCusum();
    map<string, double> getLowerCusum();
    double calcMean(vector<double> tmp_data);
    map<string, double> calcNormalizedObservations(map<string, double> subgroup_mean);
    map<string, double> calcMultipleMeans(map<string, vector<double>> subgroups);
    void calc(map<string, vector<double>> data);

};

#endif
