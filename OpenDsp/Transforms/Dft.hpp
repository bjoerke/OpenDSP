#ifndef _OPEN_DSP_DFT_H_
#define _OPEN_DSP_DFT_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Transforms/Transform.hpp>

#include <math.h>

namespace opendsp
{
template<typename Sample_T>
class Dft : public Transform<Sample_T, Complex<Sample_T>>
{
public:
    void apply(Signal<Sample_T>& src, Signal<Complex<Sample_T>>& dst) override;
    void invert(Signal<Complex<Sample_T>>& src, Signal<Sample_T>& dst) override;
};

}


template<typename Sample_T>
void opendsp::Dft<Sample_T>::apply(opendsp::Signal<Sample_T>& src, opendsp::Signal<opendsp::Complex<Sample_T>>& dst)
{
    OPEN_DSP_COND_WARNING(src.getLength() != dst.getLength(), "signals must have equal lengths");
    double factor = 2 * M_PIl / src.getLength();
    for (uint k=0 ; k<src.getLength() ; k++)
    {
        dst[k].real = 0;
        dst[k].imag = 0;
        for (uint n=0 ; n<src.getLength() ; ++n)
        {
            dst[k].real += src[n] * cos(n * k * factor);
            dst[k].imag -= src[n] * sin(n * k * factor);
        }
    }
}

template<typename Sample_T>
void opendsp::Dft<Sample_T>::invert(opendsp::Signal<opendsp::Complex<Sample_T>>& src, opendsp::Signal<Sample_T>& dst)
{
    OPEN_DSP_COND_WARNING(src.getLength() != dst.getLength(), "signals must have equal lengths");
    double factor = 2 * M_PIl / src.getLength();
    for (uint k=0 ; k<src.getLength() ; k++)
    {
        Complex<Sample_T> result(0,0);
        for (uint n=0 ; n<src.getLength() ; ++n)
        {
            Complex<Sample_T> root(cos(n * k * factor), sin(n * k * factor) );
            root *= src[n];
            result += root;
        }
        dst[k] = result.real / src.getLength();
    }
}

#endif
