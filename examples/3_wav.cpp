#include <iostream>
#include <time.h>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <OpenDsp/FrameIO/WavFileReader.hpp>
#include <OpenDsp/FrameIO/WavFileWriter.hpp>
#include <OpenDsp/Transforms/RealFft.hpp>
#include <OpenDsp/Generators/Windows.hpp>

using namespace opendsp;
using namespace std;

#define FRAME_LEN 16384
//#define FRAME_LEN 1024

int main (int argc, char *argv[])
{
    //wav test
    clock_t startTime = clock();
    Signal<double> frameIn(FRAME_LEN);
    Signal<double> frameOut(FRAME_LEN);
    Signal<double> window(FRAME_LEN);
    Windows<double>::generateBlackman(window);
    RealFft<double> fft(FRAME_LEN);
    Signal<cdouble> spectrum(fft.calcOutputLength());

    //WavFileReader<double> reader( "../OpenDSP-data/database_8.wav");
    WavFileReader<double> reader( "../OpenDSP-data/Florence & The Machine - Spectrum [cut].wav");
    //WavFileReader<double> reader( "../OpenDSP-data/Ian Carey+Snoop Dogg - Last Night.wav");
    WavFileWriter<double> writer( "../OpenDSP-data/test.wav");

    reader.open();
    writer.setSamplesPerSec(reader.getSamplesPerSec());
    writer.open();

    cout << "Bits/sample: " << reader.getBitsPerSample() << endl;
    cout << "Channels:    " << reader.getChannels() << endl;
    cout << "Samples/sec: " << reader.getSamplesPerSec() << endl;

    while(reader.hasMoreFrames())
    {
        reader.getNextFrame(frameIn);
        frameIn *= window;
        fft.apply(frameIn, spectrum);
        for(uint i=spectrum.getLength() / 32; i<spectrum.getLength() ; i++)
        {
            spectrum[i] = cdouble(0,0);
        }
        fft.invert(spectrum, frameOut);
        writer.writeFrame(frameOut);
    }
    reader.close();
    writer.close();

    printf("Time taken: %.3fs\n", (double)(clock() - startTime)/CLOCKS_PER_SEC);
    return 0;
}
