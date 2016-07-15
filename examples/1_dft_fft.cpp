#include <iostream>
#include <iomanip>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <OpenDsp/Transforms/RealFft.hpp>
#include <OpenDsp/Transforms/Fft.hpp>
#include <OpenDsp/Transforms/RealDft.hpp>
#include <OpenDsp/Transforms/Dft.hpp>

typedef float SampleType;

using namespace opendsp;
using namespace std;

int main (int argc, char *argv[])
{
    /** Do a real valued dft **/
    Signal<float> signal({1,2,3,4,5,6,7,8});
    RealFft<float> fft(signal.getLength());
    RealDft<float> dft;
    Signal<cfloat> spectrum(fft.calcOutputLength());

    cout<<"==== FFT ===="<<endl;
    fft.apply(signal, spectrum);
    cout<<"apply:  "<<spectrum<<endl;
    fft.invert(spectrum, signal);
    cout<<"invert: "<<signal<<endl;

    cout<<"===== DFT ===="<<endl;
    dft.apply(signal, spectrum);
    cout<<"apply:  "<<spectrum<<endl;
    dft.invert(spectrum, signal);
    cout<<"invert: "<<signal<<endl<<endl;

    /** Now for complex valued signals **/
    Signal<cfloat> csig({
        cfloat(1,1), cfloat(2,2), cfloat(3,3), cfloat(4,4),
        cfloat(5,5), cfloat(6,6), cfloat(7,7), cfloat(8,8)
    });
    Fft<float> cfft(csig.getLength());
    Dft<float> cdft;
    Signal<cfloat> cspec(cfft.calcOutputLength());

    cout<<"==== Complex DFT =======" <<endl;
    cdft.apply(csig, cspec);
    cout<<"apply:  "<<cspec<<endl;
    cdft.invert(cspec, csig);
    cout<<"invert: "<<csig<<endl<<endl;

    cout<<"===== Complex FFT ========"<<endl;
    cfft.apply(csig, cspec);
    cout<<"apply:  "<<cspec<<endl;
    cfft.invert(cspec,csig);
    cout<<"invert: " <<csig<<endl;


    return 0;
}
