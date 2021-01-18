#include <iostream>
#include <fstream>
#include <map>
using namespace std;
float probSrcIP(map<uint32_t, int> src_ip, int window, ofstream& traffic_analyzer){
    for (const auto& p : src_ip) {
        cout << p.first << " " << p.second << std::endl;
    }
}