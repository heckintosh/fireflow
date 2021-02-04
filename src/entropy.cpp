#include "entropy.h"
#include "mapping.h"
#include <vector>
#include <map>
#include <string>
#include <math.h>
using namespace std;

void EntropyCalc::_saveLatestEntropies()
{
    map<string, vector<double>> tmp;
    for (const auto &map_pair : entropies_of_headers)
    {
        for (int i = subgroup_size; i > 0; i--)
        {
            tmp[map_pair.first].push_back(map_pair.second[map_pair.second.size() - i]);
        }
    }
    EntropyCalc::latest_entropies = tmp;
}

map<string, vector<double>> EntropyCalc::getLatestEntropies()
{
    _saveLatestEntropies();
    PrintLatestEntropies();
    return EntropyCalc::latest_entropies;
}

void EntropyCalc::accumulate_packets(packet &current_packet)
{
    p_vector.push_back(current_packet);
}

void EntropyCalc::accumulate_subwindow_entropies(packet &current_packet)
{
    p_vector.push_back(current_packet);
    vector<double> temp;
    for (const auto &map_pair : EntropyCalc::calcMultiEntropy(count_values_all_headers(p_vector)))
    {
        entropies_of_headers[map_pair.first].push_back(map_pair.second);
    }
    p_vector.clear();
}

void EntropyCalc::zero_entropy()
{
    vector<double> temp;
    for (const auto &elem : headers)
    {
        entropies_of_headers[elem].push_back(0.0);
    }
}

map<string, double> EntropyCalc::calcMultiEntropy(map<string, map<uint, uint>> header_maps)
{

    map<string, double> entropy;
    for (const auto &outer_map_pair : header_maps)
    {
        entropy.insert({outer_map_pair.first, calcEntropy(calcProb(outer_map_pair.second))});
    }
    return entropy;
}

map<string, vector<double>> EntropyCalc::GetFullEntropies()
{
    return entropies_of_headers;
}

//given a vector of the probabilities of all values of a header
//return entropy
double EntropyCalc::calcEntropy(vector<double> probability)
{
    double size = probability.size();
    double total = 0;
    for (const auto &p : probability)
    {
        if (p == 1)
        {
            return 0;
        }
        else
        {
            total += -(p * log(p));
        }
    }
    return total / log(size);
};

// given a dictionary which contains {header_value : the number of times a header value appear)
// output a vector of probability (header_value is uint)
vector<double> EntropyCalc::calcProb(map<uint, uint> header_value_counter)
{
    double total = 0;
    vector<double> probs;
    for (const auto &p : header_value_counter)
    {
        total += p.second;
    }
    for (const auto &q : header_value_counter)
    {
        probs.push_back(q.second / total);
    }
    return probs;
};

void EntropyCalc::PrintFullEntropies()
{
    cout << "-----------FULL ENTROPIES-----------------" << endl;
    for (const auto &map_pair : entropies_of_headers)
    {
        cout << map_pair.first << ": ";
        for (const auto &entropy : map_pair.second)
        {
            cout << entropy << " ";
        }
        cout << endl;
    }
}

void EntropyCalc::PrintLatestEntropies()
{
    cout << "-----------LATEST ENTROPIES-----------------" << endl;
    for (const auto &map_pair : EntropyCalc::latest_entropies)
    {
        cout << map_pair.first << ": ";
        for (const auto &entropy : map_pair.second)
        {
            cout << entropy << " ";
        }
        cout << endl;
    }
}
