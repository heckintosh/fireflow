#ifndef ENTROPY_HEADER
#define ENTROPY_HEADER
#include <vector>
#include <map>
#include <string>
#include "packet.h"
#include "capture.h"

using namespace std;
class EntropyCalc
{
private:
    uint packet_queue = 0;
    int subgroup_size = Capture::window / Capture::subwindow;
    vector<packet> p_vector;
    vector<string> headers = {"src_ip", "src_port", "dst_port", "flags"};
    map<string, vector<double>> entropies_of_headers;
    map<string, vector<double>> latest_entropies;
    void _saveLatestEntropies();

public:
    void printEntropiesOfSubwindows();
    void accumulate_subwindow_entropies(packet &current_packet);
    void accumulate_packets(packet &current_packet);
    void threshold_estimator(int sampling_amount);
    void zero_entropy();
    double calcEntropy(vector<double> probs);
    vector<double> calcProb(map<uint, uint> header_value_counter);
    map<string, double> calcMultiEntropy(map<string, map<uint, uint>> header_maps);
    map<string, vector<double>> GetFullEntropies();
    map<string, vector<double>> GetLatestEntropies();
};
#endif
