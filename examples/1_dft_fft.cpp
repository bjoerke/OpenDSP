#include <iostream>
#include <iomanip>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <OpenDsp/Transforms/Fft.hpp>
#include <OpenDsp/Transforms/Dft.hpp>

typedef float SampleType;

using namespace opendsp;
using namespace std;

int main (int argc, char *argv[])
{
    const uint len = 8;
    Signal<float> signal({1,2,3,4,5,6,7,8});
    /*
    for(uint i=0; i<len; i++)
    {
        signal[i] = sin(i);
    }
    */
    cout<<signal<<endl;
    //TODO: real ist achsensymmetrisch, imag ist punktsymmetrisch fuer relles signal

    Signal<cfloat> spectrum(signal.getLength());

    cout<<"==== FFT ===="<<endl;
    Fft<float> fft(signal.getLength());
    spectrum = cfloat(0,0);
    fft.apply(signal, spectrum);
    cout<<spectrum<<endl;
    signal = 0;
    fft.invert(spectrum, signal);
    cout<<signal<<endl;

    cout<<"===== DFT ===="<<endl;
    Dft<float> dft;
    spectrum = cfloat(0,0);
    dft.apply(signal, spectrum);
    cout<<spectrum<<endl;
    signal = 0;
    dft.invert(spectrum, signal);
    cout<<signal<<endl;
    cout<<endl;

    return 0;
}
