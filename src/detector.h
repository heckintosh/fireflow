#ifndef DETECTOR_HEADER
#define DETECTOR_HEADER
#include "cusum.h"
using namespace std;
class Detector
{
private:
    Cusum cusum_stats;
    bool isHigh = true;
public:
    Detector(Cusum cusum_stats);
};
#endif