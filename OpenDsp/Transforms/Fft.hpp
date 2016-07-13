#ifndef _OPEN_DSP_FFT_H_
#define _OPEN_DSP_FFT_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Transforms/Transform.hpp>
#include <OpenDsp/Math/Complex.hpp>
#include <math.h>

namespace opendsp
{

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

template<typename Sample_T>
class Fft : public Transform<Complex<Sample_T>, Complex<Sample_T> >
{
    public:
        Fft(uint length);
        ~Fft();
        void apply(Signal<Complex<Sample_T>>& in, Signal<Complex<Sample_T>>& out) override;
        void invert(Signal<Complex<Sample_T>>& in, Signal<Complex<Sample_T>>& out) override;

        void transform(Signal<Complex<Sample_T>>& in, Signal<Complex<Sample_T>>& out, bool invert);

    public: //TODO: make friend to RealFft
        uint length;
        uint* reversedBits;

    private:
        uint steps;
        Complex<Sample_T>* twiddles;

};
}

template<typename SampleType>
opendsp::Fft<SampleType>::Fft(uint length) :
    length{length},
    reversedBits{new uint[length]},
    steps{(uint) (log(length) / log(2))}  //TODO: faster way http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious
{
    OPEN_DSP_COND_WARNING( !isPowerOfTwo(length), "length must be a power of 2");

    //calculate twiddle factors (roots of unity)
    uint numTwiddles = length / 2;  //half, because twiddle(i+N/2) = -twiddle(i)
    twiddles = new Complex<SampleType>[numTwiddles];
    for(uint i=0; i<numTwiddles; i++)
    {
        SampleType arg = M_PI*i/numTwiddles;
        twiddles[i].real = cos(arg);
        twiddles[i].imag = -sin(arg);
        //std::cout<<twiddles[i]<<std::endl;
    }

    //calc reversed bits
    for(uint i=0; i<length; i++)
    {
        reversedBits[i] = reverseBits(i, steps);
        //std::cout<<reversedBits[i];
    }
}

template<typename SampleType>
opendsp::Fft<SampleType>::~Fft()
{
    delete[] twiddles;
}

template<typename Sample_T>
void opendsp::Fft<Sample_T>::apply(Signal<Complex<Sample_T>>& in, Signal<Complex<Sample_T>>& out)
{
    Complex<Sample_T> temp;
    for(uint i=0; i<length; i++)
    {
        uint rev = reversedBits[i];
        if(rev > i)
        {
            temp = in[i];
            out[i] = in[rev];
            out[rev] = temp;
        }
    }
    transform(in, out, false);
}

template<typename Sample_T>
void opendsp::Fft<Sample_T>::invert(Signal<Complex<Sample_T>>& in, Signal<Complex<Sample_T>>& out)
{
    Complex<Sample_T> temp;
    for(uint i=0; i<length; i++)
    {
        uint rev = reversedBits[i];
        if(rev > i)
        {
            temp = in[i];
            out[i] = in[rev];
            out[rev] = temp;
        }
    }
    transform(in, out, true);
    for(uint i=0; i<length; i++)
    {
        out[i] /= length;
    }
}


/**
 * Complex valued fft
 * input signal must be in bit-reversed order
 */
template<typename SampleType>
void opendsp::Fft<SampleType>::transform(opendsp::Signal<opendsp::Complex<SampleType>>& in, opendsp::Signal<opendsp::Complex<SampleType>>& out, bool invert)
{
    //traverse butterfly graph
    for(uint step=0; step<steps; step++)
    {
        uint parts = 1<<(steps-step-1);
        uint elements = length / parts;
        // std::cout<<"step "<<step<<" has "<<parts<<" parts"<<std::endl;
        for(uint part=0; part<parts; part++)
        {
            // std::cout<<"\tpart " << part<<std::endl;
            for(uint element=0; element < elements / 2; element++)
            {
                uint left =  part*elements + element;
                uint right = left + elements / 2;
                uint twiddleIndex = element * parts;
                Complex<SampleType> rightValue = in[right];
                Complex<SampleType> twiddle = twiddles[twiddleIndex];
                if(invert)
                {
                    twiddle.conjugate();
                }
                rightValue *= twiddle;
                out[right] = in[left];
                out[right] -= rightValue;
                out[left]  += rightValue;
                // std::cout<<"\t\tcombine "<<left<<" and "<<right;
                // std::cout<<" with twiddle "<<twiddle<<" yields"<<dst[left]<<" "<<dst[right]<<std::endl;
            }
        }
    }
}

#endif
