#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <sstream>

using namespace std;

void countSourceIP_Map(map<string, int> &src_ip_map, string src_ip){
    if (src_ip_map.count(src_ip)){
        src_ip_map[src_ip] += 1;
    }
    else if (!src_ip_map.count(src_ip)){
        src_ip_map.insert({src_ip, 1});
    }
    else{
        cout << "Error when processing and counting IPs";
    }
}

void log_packet_summary(queue<string> &src_ip_queue){
    map<string, int>src_ip;
    while (!src_ip_queue.empty()){
        countSourceIP_Map(src_ip, src_ip_queue.front()); 
        src_ip_queue.pop();  
    }
    for (const auto& p : src_ip) {
        std::cout << p.first << " " << p.second << std::endl; // "Karl", "George"
    }
}


int main(){
    queue<string> src_ip_queue;
    src_ip_queue.push("192.168.1.1");
    src_ip_queue.push("192.168.1.2");
    src_ip_queue.push("192.168.1.1");
    log_packet_summary(src_ip_queue);
}
