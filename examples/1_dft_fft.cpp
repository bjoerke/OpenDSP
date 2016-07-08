#include <iostream>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>

typedef float SampleType;

using namespace opendsp;
using namespace std;

int main (int argc, char *argv[])
{
    const uint len = 64;
    Signal<float> signal(len);
    for(uint i=0; i<len; i++)
    {
        signal[i] = sin(i);
    }
    cout<<signal<<endl;
    //TODO: real ist achsensymmetrisch, imag ist punktsymmetrisch fuer relles signal

    Signal<cfloat> spectrum(signal.getLength());

    cout<<"===== DFT ===="<<endl;
    signal.dft(spectrum);
    cout<<spectrum<<endl;
    cout<<endl;

    cout<<"==== FFT ===="<<endl;
    signal.fft(spectrum);
    cout<<spectrum<<endl<<endl;
    for(uint i=0; i<spectrum.getLength(); i++)
    {
        cout<<(int)spectrum[i].amplitude()<<", ";
    }
    cout<<endl;

    return 0;
}

/** DFT examples:

1,2,3,4 =>
	10 + 0j
	-2 + 2j
	-2 + 0j
	-2 - 2j

1,0,1,0 =>
1	2 + 0j
2	0 + 0j
3	2 + 0j
4	0 + 0j

1,2,3,4,5,6,5,4,3,2,1 ==>
1	36 + 0j
2	-11.843538 - 3.477576j
3	0.228446 + 0.146813j
4	-0.948691 - 1.094847j
5	0.146747 + 0.32133j
6	-0.082964 - 0.577031j
7	-0.082964 + 0.577031j
8	0.146747 - 0.32133j
9	-0.948691 + 1.094847j
10	0.228446 - 0.146813j
11	-11.843538 + 3.477576j

**/
