#ifndef _OPEN_DSP_SIGNAL_H_
#define _OPEN_DSP_SIGNAL_H_

#include <OpenDsp/OpenDsp.hpp>
#include <string>
#include <sstream>
#include <math.h>

namespace opendsp
{
template<typename SampleType>
class Signal
{
public:

    Signal(uint length);
    Signal(SampleType* samples, uint length);
    Signal(std::initializer_list<SampleType>);
    Signal(const Signal<SampleType>& copy);
    ~Signal();

    SampleType& operator[](uint index);
    const SampleType& operator[](uint index) const;

    //Signal<SampleType>& convolve(Signal<SampleType> h) const;
    void convolve(const Signal<SampleType>& h, Signal<SampleType>& dst) const;
    void convolveWithOverlap(const Signal<SampleType>& h, Signal<SampleType>& dst, Signal<SampleType>& overlap) const;
    uint calcConvolutionResultLength(const Signal<SampleType>& h) const;
    uint calcConvolutionOverlapLength(const Signal<SampleType>& h) const;

    Signal<SampleType>& operator=(Signal<SampleType> copy);
    uint getLength() const;

    virtual const std::string toString() const;


    void dft(Signal<Complex<SampleType>>& dst) const;
    void fft(Signal<Complex<SampleType>>& dst) const;

private:
    SampleType* samples;
    uint length;

};

}

/**
 * Converts the signal to it's string representation and sends it to the stream
 */
template<typename SampleType>
std::ostream& operator<<(std::ostream& os, const opendsp::Signal<SampleType>& sig);

//TODO assignement constructor

template<typename SampleType>
opendsp::Signal<SampleType>::Signal(uint length) :
    samples{new SampleType[length]},
    length{length}
{
    for(uint i=0; i<length; i++)
    {
        samples[i] = 0; //TODO memset
    }
}

/**
 * Constructs a signal of a specific length; all samples are initialized to 0
 * @param length length of the signal
 */
template<typename SampleType>
opendsp::Signal<SampleType>::Signal(SampleType* samples, uint length) :
    samples{new SampleType[length]},
    length{length}
{
    for(uint i=0; i<length; i++)
    {
        //TODO memcpy!
        this->samples[i] = samples[i];
    }
}

/**
 * Constructs a signal with initial sample values
 * @param list a list of sample values, e.g. {1,3,4}
 */
template<typename SampleType>
opendsp::Signal<SampleType>::Signal(std::initializer_list<SampleType> list) :
    samples{new SampleType[list.size()]},
    length{(uint) list.size()}
{
   std::copy(list.begin(), list.end(), samples);
}

/**
 * Copy constructor
 * @param copy signal to copy the samples from
 */
template<typename SampleType>
opendsp::Signal<SampleType>::Signal(const opendsp::Signal<SampleType>& copy) :
    samples{new SampleType[copy.length]},
    length{copy.length}
{
    OPEN_DSP_WARNING("copy constructor called");
    for(uint i=0; i<length; i++)
    {
        samples[i] = copy[i]; //TODO std::copy?
    }
}
/**
 * Destructor
 */
template<typename SampleType>
opendsp::Signal<SampleType>::~Signal()
{
    delete[] samples;
}

template<typename SampleType>
inline SampleType& opendsp::Signal<SampleType>::operator[](uint index)
{
    return samples[index];
}

template<typename SampleType>
inline const SampleType& opendsp::Signal<SampleType>::operator[](uint index) const
{
    return samples[index];
}

template<typename SampleType>
opendsp::Signal<SampleType>& opendsp::Signal<SampleType>::operator=(opendsp::Signal<SampleType> copy)
{
    OPEN_DSP_WARNING("assignement called but not implemented!");
}

template<typename SampleType>
void opendsp::Signal<SampleType>::convolve(const Signal<SampleType>& in, Signal<SampleType>& dst) const
{
    uint resultLength = calcConvolutionResultLength(in);
    OPEN_DSP_COND_WARNING(dst.length != resultLength, "dst has wrong size");

    // we move the shorter signal over the longer one to improve performance
    SampleType* x; SampleType* h;
    uint xLen; uint hLen;
    if(this->length > in.length)
    {
        x = this->samples; xLen = this->length;
        h = in.samples;    hLen = in.length;
    }
    else
    {
        x = in.samples;    xLen = in.length;
        h = this->samples; hLen = this->length;
    }

    //iterate over each output pixel
    for(uint n=0; n<resultLength; n++)
    {
        dst.samples[n] = 0;

        //signal h is inverted and shifted by n to the right
        uint mMin = (n >= hLen - 1) ? n - (hLen - 1) : 0;  //x is zero before n=0; no need to compute
        uint mMax = (n < xLen - 1) ? n : xLen- 1; //x is zero after xLen; no need to compute
        for(uint m=mMin; m<=mMax; m++)
        {
           dst.samples[n] += x[m] * h[n-m];
        }
    }
}

template<typename SampleType>
void opendsp::Signal<SampleType>::convolveWithOverlap(const Signal<SampleType>& in, Signal<SampleType>& dst, Signal<SampleType>& overlap) const
{
    OPEN_DSP_COND_WARNING(dst.length != this->length, "dst has wrong length");
    OPEN_DSP_COND_WARNING(in.length > this->length, "in is too long");

    uint resultLength = calcConvolutionResultLength(in);
    uint xLen = this->length; uint hLen = in.length;
    SampleType* x = this->samples;
    SampleType* h = in.samples;

    //first part: fill "dst"
    for(uint n=0; n<xLen; n++)
    {
        dst.samples[n] = (n < overlap.length) ? overlap.samples[n] : 0;

        //signal h is inverted and shifted by n to the right
        uint mMin = (n >= hLen - 1) ? n - (hLen - 1) : 0;  //x is zero before n=0; no need to compute
        uint mMax = n; //x is zero after xLen; no need to compute
        for(uint m=mMin; m<=mMax; m++)
        {
           dst.samples[n] += x[m] * h[-(m-n)];
        }
    }

    //second part: fill "overlap"
    for(uint n=xLen; n<resultLength; n++)
    {
        overlap.samples[n-xLen] = 0;

        //signal h is inverted and shifted by n to the right
        uint mMin = (n >= hLen - 1) ? n - (hLen - 1) : 0;  //x is zero before n=0; no need to compute
        uint mMax = xLen- 1; //x is zero after xLen; no need to compute
        for(uint m=mMin; m<=mMax; m++)
        {
           overlap.samples[n-xLen] += x[m] * h[-(m-n)];
        }
    }
}

template<typename SampleType>
uint opendsp::Signal<SampleType>::calcConvolutionResultLength(const Signal<SampleType>& h) const
{
    return this->length + h.length - 1;
}

template<typename SampleType>
uint opendsp::Signal<SampleType>::calcConvolutionOverlapLength(const Signal<SampleType>& h) const
{
    return h.length - 1;
}

template<typename SampleType>
void opendsp::Signal<SampleType>::dft(opendsp::Signal<opendsp::Complex<SampleType>>& dst) const
{
    double factor = 2 * M_PIl / length;
    for (uint k=0 ; k<length ; k++)
    {
        dst[k].real = 0;
        dst[k].imag = 0;
        for (uint n=0 ; n<length ; ++n)
        {
            dst[k].real += samples[n] * cos(n * k * factor);
            dst[k].imag -= samples[n] * sin(n * k * factor);
        }
    }
}

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

template<typename SampleType>
void opendsp::Signal<SampleType>::fft(opendsp::Signal<opendsp::Complex<SampleType>>& dst) const
{
    uint steps = log(length) / log(2);  //TODO: faster way http://graphics.stanford.edu/~seander/bithacks.html#IntegerLogObvious

    //store samples at bit-reversed index in dst
    for(uint i=0; i<length; i++)
    {
        dst[i] = samples[reverseBits(i,steps)];
    }

    //calculate twiddle factors (roots of unity)
    uint numTwiddles = dst.getLength() / 2;  //half, because twiddle(i+N/2) = -twiddle(i)
    Complex<SampleType>* twiddles = new Complex<SampleType>[numTwiddles];
    for(uint i=0; i<numTwiddles; i++)
    {
        SampleType arg = M_PI*i/numTwiddles;
        twiddles[i].real = cos(arg);
        twiddles[i].imag = sin(arg);
       // std::cout<<twiddles[i]<<std::endl;
    }

    //traverse butterfly graph
    for(uint step=0; step<steps; step++)
    {
        uint parts = 1<<(steps-step-1);
        uint elements = length / parts;
        //std::cout<<"step "<<step<<" has "<<parts<<" parts"<<std::endl;
        for(uint part=0; part<parts; part++)
        {
            //std::cout<<"\tpart " << part<<std::endl;
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
                //std::cout<<"\t\tcombine "<<left<<" and "<<right;
                //std::cout<<" with twiddle "<<twiddle<<" yields"<<dst[left]<<" "<<dst[right]<<std::endl;
            }
        }
    }
}

//TODO Goertzel-Algorithmus

template<typename SampleType>
const std::string opendsp::Signal<SampleType>::toString() const
{
    std::stringstream ss;
    ss << length <<": ";
    for(uint i=0; i<length; i++)
    {
        ss << samples[i];
        if(i<length-1)
        {
            ss << ", ";
        }
    }
    return ss.str();
}

template<typename SampleType>
uint opendsp::Signal<SampleType>::getLength() const
{
    return length;
}

template<typename SampleType>
std::ostream& operator<<(std::ostream& os, const opendsp::Signal<SampleType>& sig)
{
    os << sig.toString();
    return os;
}

#endif
