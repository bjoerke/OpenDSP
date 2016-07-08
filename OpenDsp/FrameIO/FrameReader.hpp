#ifndef _OPEN_DSP_SIGNAL_SOURCE_H_
#define _OPEN_DSP_SIGNAL_SOURCE_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>

namespace opendsp
{
    template<typename SampleType>
    class FrameReader
    {
        public:
            virtual void open() = 0;
            virtual void close() = 0;
            virtual bool hasMoreFrames() const = 0;
            virtual void getNextFrame(Signal<SampleType>& dst) = 0;
            virtual ~FrameReader() { ; }
    };
}

#endif
