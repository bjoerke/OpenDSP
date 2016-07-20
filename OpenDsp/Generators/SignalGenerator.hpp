#ifndef _OPENDSP_SIGNAL_GEN_H_
#define _OPENDSP_SIGNAL_GEN_H_

#include <OpenDsp/OpenDsp.hpp>

namespace opendsp
{
template<typename SampleType>
class SignalGenerator
{
    virtual void generateNextFrame(Signal<SampleType>& frame) = 0;

};

}

#endif
