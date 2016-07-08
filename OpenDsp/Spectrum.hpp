#ifndef _OPEN_DSP_SPECTRUM_H_
#define _OPEN_DSP_SPECTRUM_H_

#include "OpenDsp.hpp"
#include <OpenDsp/SampleContainer.hpp>
#include <string>
#include <sstream>

namespace opendsp
{
template<typename SampleType>
class Spectrum : public SampleContainer<Complex<SampleType>>
{

};

}

#endif
