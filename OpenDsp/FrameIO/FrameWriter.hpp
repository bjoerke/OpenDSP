#ifndef _OPEN_DSP_FRAME_READER_H_
#define _OPEN_DSP_FRAME_READER_H_

#include <OpenDsp/OpenDsp.hpp>

namespace opendsp
{
template<typename SampleType>
class FrameWriter
{
    public:
        virtual void writeFrame(Signal<SampleType>& frame) = 0;
        virtual void open() = 0;
        virtual void close() = 0;
        virtual ~FrameWriter() {;}
};
}

#endif
