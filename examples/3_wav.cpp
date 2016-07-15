#include <iostream>
#include <time.h>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <OpenDsp/FrameIO/WavFileReader.hpp>
#include <OpenDsp/FrameIO/WavFileWriter.hpp>
#include <OpenDsp/Transforms/RealFft.hpp>

using namespace opendsp;
using namespace std;

#define FRAME_LEN 1048576

int main (int argc, char *argv[])
{
    //wav test
    clock_t startTime = clock();
    Signal<double> frameIn(FRAME_LEN);
    Signal<double> frameOut(FRAME_LEN);
    Signal<cdouble> spectrum(FRAME_LEN);
    RealFft<double> fft(FRAME_LEN);

    //WavFileReader<double> reader( "../OpenDSP-data/database_8.wav");
    WavFileReader<double> reader( "../OpenDSP-data/Florence & The Machine - Spectrum (Say My Name) (Calvin Harris Extended Mix).wav");
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
        fft.apply(frameIn, spectrum);
        for(uint i=FRAME_LEN / 32; i<31*FRAME_LEN / 32 ; i++)
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
