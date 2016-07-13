#include <iostream>
#include <iomanip>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <OpenDsp/Transforms/RealFft.hpp>
#include <OpenDsp/Transforms/Dft.hpp>

typedef float SampleType;

using namespace opendsp;
using namespace std;

int main (int argc, char *argv[])
{
    Signal<float> signal({1,2,3,4,5,6,7,8});
    RealFft<float> fft(signal.getLength());
    Dft<float> dft;
    Signal<cfloat> spectrum(fft.calcOutputLength());

    cout<<"==== FFT ===="<<endl;
    spectrum = cfloat(0,0);
    fft.apply(signal, spectrum);
    cout<<spectrum<<endl;
    signal = 0;
    fft.invert(spectrum, signal);
    cout<<signal<<endl;

    cout<<"===== DFT ===="<<endl;
    spectrum = cfloat(0,0);
    dft.apply(signal, spectrum);
    cout<<spectrum<<endl;
    signal = 0;
    dft.invert(spectrum, signal);
    cout<<signal<<endl;
    cout<<endl;

    return 0;
}
