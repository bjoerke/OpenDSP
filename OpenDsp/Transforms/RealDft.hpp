#ifndef _OPEN_DSP_REAL_DFT_H_
#define _OPEN_DSP_REAL_DFT_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Transforms/Transform.hpp>
#include <OpenDsp/Math/Complex.hpp>

namespace opendsp
{
template<typename Sample_T>
class RealDft : public Transform<Sample_T, Complex<Sample_T>>
{
    public:
        void apply(Signal<Sample_T>&  in, Signal<Complex<Sample_T>>& out) override;
        void invert(Signal<Complex<Sample_T>>& in, Signal<Sample_T>& out) override;
        uint calcOutputLength(uint length);

    private:
        uint length;
};

}

template<typename Sample_T>
uint opendsp::RealDft<Sample_T>::calcOutputLength(uint length)
{
    return length/2+1;
}


template<typename Sample_T>
void opendsp::RealDft<Sample_T>::apply(opendsp::Signal<Sample_T>& in, opendsp::Signal<opendsp::Complex<Sample_T>>& out)
{
    OPEN_DSP_COND_WARNING(calcOutputLength(in.getLength()) != out.getLength(), "invalid lengths");
    double factor = 2 * M_PIl / in.getLength();
    for (uint k=0 ; k<out.getLength(); k++)
    {
        out[k] = 0;
        for (uint n=0 ; n<in.getLength() ; ++n)
        {
            Complex<Sample_T> root(cos(n * k * factor), -sin(n * k * factor) );
            root *= in[n];
            out[k] += root;
        }
    }
}

template<typename Sample_T>
void opendsp::RealDft<Sample_T>::invert(opendsp::Signal<opendsp::Complex<Sample_T>>& in, opendsp::Signal<Sample_T>& out)
{
    OPEN_DSP_COND_WARNING(calcOutputLength(out.getLength()) != in.getLength(), "invalid lengths");
    double factor = 2 * M_PIl / out.getLength();
    for (uint k=0 ; k<out.getLength(); k++)
    {
        Complex<Sample_T> result(0,0);
        Complex<Sample_T> mul;
        for (uint n=0 ; n<out.getLength() ; ++n)
        {
            Complex<Sample_T> root(cos(n * k * factor), sin(n * k * factor) );
            if(n < in.getLength())
            {
                mul = in[n];
            }
            else
            {
                mul = in[out.getLength() - n];
                mul.conjugate();
            }
            root *= mul;
            result += root;
        }
        out[k] = result.real / out.getLength();
    }
}

#endif
