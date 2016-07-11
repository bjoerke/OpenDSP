#include <iostream>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <OpenDsp/Transforms/Fft.hpp>

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
    fft.apply(signal, spectrum);
    cout<<spectrum<<endl<<endl;
    /**
    for(uint i=0; i<spectrum.getLength(); i++)
    {
        cout<<(int)spectrum[i].amplitude()<<", ";
    }
    cout<<endl;*/
    cout<<"===== DFT ===="<<endl;
    signal.dft(spectrum);
    cout<<spectrum<<endl;
    cout<<endl;

    return 0;
}
