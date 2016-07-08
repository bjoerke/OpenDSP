#ifndef _OPENDSP_SINE_GEN_H_
#define _OPENDSP_SINE_GEN_H_

#include <math.h>

#include <OpenDsp/SignalGenerators/SignalGenerator.hpp>

namespace opendsp
{
template<typename SampleType>
class SineGenerator : public SignalGenerator<SampleType>
{
public:
    SineGenerator(float frequency, float phase)
    {

    }
    void generateNextFrame(Signal<SampleType>& frame)
    {
        ;
    }
private:
    float frequency = 1;
    float phase = 0;
    int currentSample;
};
}

#endif
