#ifndef _OPEN_DSP_FFT_H_
#define _OPEN_DSP_FFT_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Math/Complex.hpp>
#include <math.h>

//TODO improve performance!
static uint reverseBits(uint val, uint bits)
{
    uint ret = 0;
    for(uint i=0; i<bits; i++)
    {
        ret = ret << 1;
        ret |= val & 0x01;
        val = val >> 1;
    }
    return ret;
}


namespace opendsp
{
template<typename SampleType>
class Fft
{
    public:
        Fft(uint length);
        void apply(Signal<SampleType>& src, Signal<Complex<SampleType>>& dst);
        void invert(Signal<Complex<SampleType>>& src, Signal<SampleType>& dst);

    private:
        uint length;
        uint steps;
        Complex<SampleType>* twiddles;
        uint* reversedBits;
};

}

template<typename SampleType>
opendsp::Fft<SampleType>::Fft(uint length) :
    length{length},
    steps{(uint) (log(length) / log(2))}  //TODO: faster way http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
{
    //calculate twiddle factors (roots of unity)
    uint numTwiddles = length / 2;  //half, because twiddle(i+N/2) = -twiddle(i)
    twiddles = new Complex<SampleType>[numTwiddles];
    for(uint i=0; i<numTwiddles; i++)
    {
        SampleType arg = M_PI*i/numTwiddles;
        twiddles[i].real = cos(arg);
        twiddles[i].imag = -sin(arg);
        std::cout<<twiddles[i]<<std::endl;
    }

    //calc reversed bits
    reversedBits = new uint[length];
    for(uint i=0; i<length; i++)
    {
        reversedBits[i] = reverseBits(i, steps);
        //std::cout<<reversedBits[i];
    }
}

template<typename SampleType>
void opendsp::Fft<SampleType>::apply(opendsp::Signal<SampleType>& src, opendsp::Signal<opendsp::Complex<SampleType>>& dst)
{
    //store samples at bit-reversed index in dst
    for(uint i=0; i<length; i++)
    {
        dst[i] = src[reversedBits[i]];
    }

    //traverse butterfly graph
    for(uint step=0; step<steps; step++)
    {
        uint parts = 1<<(steps-step-1);
        uint elements = length / parts;
        std::cout<<"step "<<step<<" has "<<parts<<" parts"<<std::endl;
        for(uint part=0; part<parts; part++)
        {
            std::cout<<"\tpart " << part<<std::endl;
            for(uint element=0; element < elements / 2; element++)
            {
                uint left =  part*elements + element;
                uint right = left + elements / 2;
                uint twiddle = element * parts;
                Complex<SampleType> rightValue = dst[right];
                rightValue *= twiddles[twiddle];
                dst[right] = dst[left];
                dst[right] -= rightValue;
                dst[left]  += rightValue;
                std::cout<<"\t\tcombine "<<left<<" and "<<right;
                std::cout<<" with twiddle "<<twiddle<<" yields"<<dst[left]<<" "<<dst[right]<<std::endl;
            }
        }
    }

}


template<typename SampleType>
void opendsp::Fft<SampleType>::invert(Signal<Complex<SampleType>>& src, Signal<SampleType>& dst)
{
    ;
}


#endif
