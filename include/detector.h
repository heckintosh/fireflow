#ifndef DETECTOR_HEADER
#define DETECTOR_HEADER
#include "cusum.h"
using namespace std;
class Detector
{
public:
    void judge(Cusum cusum_object);
};
#endif