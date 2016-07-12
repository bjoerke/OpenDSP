#ifndef _OPEN_DSP_TRANSFORM_H_
#define _OPEN_DSP_TRANSFORM_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>

namespace opendsp
{
/**
 * Base class for all transforms
 * A transform converts a Signal from one space to an other
 * Examples: FFT, DFT, DCT, ...
 */
template<typename Input_T, typename Output_T>
class Transform
{
    public:
        /**
         * Transforms a signal
         * @param in input signal
         * @param out output signal
         */
        virtual void  apply(Signal<Input_T>& in, Signal<Output_T>& out) = 0;
        /**
         * Transforms a signal back to it's original
         * @param in input signal
         * @param out output signal
         */
        virtual void invert(Signal<Output_T>& in, Signal<Input_T>& out) = 0;
};

}

#endif
