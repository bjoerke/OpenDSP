#ifndef _OPEN_DSP_REAL_FFT_H_
#define _OPEN_DSP_REAL_FFT_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Transforms/Transform.hpp>
#include <OpenDsp/Transforms/Fft.hpp>
#include <OpenDsp/Math/Complex.hpp>
#include <math.h>


namespace opendsp
{
template<typename Sample_T>
class RealFft : public Transform<Sample_T, Complex<Sample_T>>
{
public:
    RealFft(uint length);
    void apply(Signal<Sample_T>& src, Signal<Complex<Sample_T>>& dst) override;
    void invert(Signal<Complex<Sample_T>>& src, Signal<Sample_T>& dst) override;
    virtual ~RealFft();

    /**
     * Calcualtes the length of the transformed signal
     */
    uint calcOutputLength();

private:
    Signal<Complex<Sample_T>>* buffer;
    Fft<Sample_T> fft;

};

}

template<typename SampleType>
opendsp::RealFft<SampleType>::RealFft(uint length) :
    buffer{new Signal<Complex<SampleType>>(length)},
    fft(length)
{
    ;
}

template<typename SampleType>
opendsp::RealFft<SampleType>::~RealFft()
{
    delete buffer;
}

template<typename SampleType>
void opendsp::RealFft<SampleType>::apply(opendsp::Signal<SampleType>& src, opendsp::Signal<opendsp::Complex<SampleType>>& dst)
{
    //store samples at bit-reversed index in dst
    for(uint i=0; i<fft.getLength(); i++)
    {
        (*buffer)[i] = src[i];
    }
    fft.apply(*buffer, *buffer);
    //for a real valued input the spectrum is hermetic
    for(uint i=0; i<fft.getLength()/2+1; i++)
    {
        dst[i] = (*buffer)[i];
    }
}


template<typename SampleType>
void opendsp::RealFft<SampleType>::invert(Signal<Complex<SampleType>>& src, Signal<SampleType>& dst)
{
    //make signal symmetric
    for(uint i=0; i<fft.getLength()/2+1; i++)
    {
       (*buffer)[i] = src[i];
    }
    for(uint i=fft.getLength()/2+1; i<fft.getLength(); i++)
    {
        (*buffer)[i] = src[fft.getLength() - i];
        (*buffer)[i].conjugate();
    }
    fft.invert(*buffer, *buffer);
    for(uint i=0; i<fft.getLength(); i++)
    {
        dst[i] = (*buffer)[i].real;
    }
}

template<typename Sample_T>
uint opendsp::RealFft<Sample_T>::calcOutputLength()
{
    return fft.getLength()/2+1;
}

#endif
