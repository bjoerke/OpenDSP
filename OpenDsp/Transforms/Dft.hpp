#ifndef _OPEN_DSP_DFT_H_
#define _OPEN_DSP_DFT_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Transforms/Transform.hpp>
#include <stdbool.h>
#include <math.h>

namespace opendsp
{
template<typename Sample_T>
class Dft : public Transform<Complex<Sample_T>, Complex<Sample_T>>
{
public:
    void apply(Signal<Complex<Sample_T>>& src, Signal<Complex<Sample_T>>& dst) override;
    void invert(Signal<Complex<Sample_T>>& src, Signal<Complex<Sample_T>>& dst) override;
private:
    void transform(Signal<Complex<Sample_T>>& src, Signal<Complex<Sample_T>>& dst, bool invert);

};

}

template<typename Sample_T>
void opendsp::Dft<Sample_T>::transform(opendsp::Signal<opendsp::Complex<Sample_T>>& src, opendsp::Signal<opendsp::Complex<Sample_T>>& dst, bool invert)
{
    OPEN_DSP_COND_WARNING(src.getLength() != dst.getLength(), "signals must have equal lengths");
    double factor = 2 * M_PIl / src.getLength();
    for (uint k=0 ; k<src.getLength() ; k++)
    {
        Complex<Sample_T> result(0,0);
        for (uint n=0 ; n<src.getLength() ; ++n)
        {
            Complex<Sample_T> root(cos(n * k * factor), -sin(n * k * factor) );
            if(invert)
            {
                root.conjugate();
            }
            root *= src[n];
            result += root;
        }
        if(invert)
        {
            result /= src.getLength();
        }
        dst[k] = result;
    }
}

template<typename Sample_T>
void opendsp::Dft<Sample_T>::apply(opendsp::Signal<opendsp::Complex<Sample_T>>& src, opendsp::Signal<opendsp::Complex<Sample_T>>& dst)
{
    transform(src, dst, false);
}

template<typename Sample_T>
void opendsp::Dft<Sample_T>::invert(opendsp::Signal<opendsp::Complex<Sample_T>>& src, opendsp::Signal<opendsp::Complex<Sample_T>>& dst)
{
    transform(src, dst, true);
}

#endif
