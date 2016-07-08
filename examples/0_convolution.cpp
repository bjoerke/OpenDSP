#include <iostream>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>

typedef float SampleType;

using namespace opendsp;
using namespace std;

int main (int argc, char *argv[])
{
    Signal<SampleType> frameX({1,2,3,4,5,6,7,6,5,4,3,2,1});
    Signal<SampleType> frameY({1,2,3,2,1});
    Signal<SampleType> frameZ(frameX.calcConvolutionResultLength(frameY));
    frameX.convolve(frameY, frameZ);
    cout << frameZ << endl;

    Signal<SampleType> overlap(frameX.calcConvolutionOverlapLength(frameY));
    Signal<SampleType> result(frameX.getLength());
    frameX.convolveWithOverlap(frameY, result, overlap);
    cout << result << overlap << endl;
    return 0;
}
