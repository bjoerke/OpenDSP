#ifndef _OPEN_DSP_WINDOWS_H_
#define _OPEN_DSP_WINDOWS_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <math.h>

namespace opendsp
{
template<typename Sample_T>
class Windows
{
public:
    static void generateHamming(Signal<Sample_T>& dst)
    {
        for(uint i=0; i<dst.getLength(); i++)
        {
            dst[i] = 0.54 - 0.46 * cos(2*M_PI*i / dst.getLength());
        }
    }

    static void generateBlackman(Signal<Sample_T>& dst)
    {
        for(uint i=0; i<dst.getLength(); i++)
        {
            dst[i] = 0.52 - 0.5 * cos(2*M_PI*i / dst.getLength()) + 0.08 * cos(4*M_PI*i/dst.getLength()) ;
        }
    }
};

}

#endif
