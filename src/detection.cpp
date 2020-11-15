#include <fstream>
#include <iostream>
#include <string>
using namespace std;

void readLogFile(int lines, string filename);
void readLogFile(int lines, string filename){
    ifstream packet_file;
    packet_file.open(filename);
    string line;
    if (packet_file.is_open()){
        while (getline(packet_file, line)){
            cout << line;
        }
    }
}

int main(){
    string filename = "/tmp/packetlogger.txt";
    readLogFile(10, filename );
}