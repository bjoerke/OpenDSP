#include <iostream>
#include <time.h>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <OpenDsp/FrameIO/WavFileReader.hpp>
#include <OpenDsp/FrameIO/WavFileWriter.hpp>

using namespace opendsp;
using namespace std;

typedef double SampleType;

int main (int argc, char *argv[])
{
    //wav test
    clock_t startTime = clock();
    Signal<SampleType> dirac(14); dirac[13] = 1;
    Signal<SampleType> frameIn(1024);
    Signal<SampleType> frameOut(1024);
    Signal<SampleType> frameOverlap(frameIn.calcConvolutionOverlapLength(dirac));
   // for(int i=0; i<1024; i++) frameOut[i] = i*30;

    WavFileReader<SampleType> reader( "database_8.wav");
    WavFileWriter<SampleType> writer( "test.wav");

    reader.open();
    writer.setSamplesPerSec(reader.getSamplesPerSec());
    writer.open();

    cout << "Bits/sample: " << reader.getBitsPerSample() << endl;
    cout << "Channels:    " << reader.getChannels() << endl;
    cout << "Samples/sec: " << reader.getSamplesPerSec() << endl;

    while(reader.hasMoreFrames())
    {
        reader.getNextFrame(frameIn);
        frameIn.convolveWithOverlap(dirac, frameOut, frameOverlap);
        writer.writeFrame(frameOut);
    }
    reader.close();
    writer.close();

    printf("Time taken: %.3fs\n", (double)(clock() - startTime)/CLOCKS_PER_SEC);
    cin.get();
    return 0;
}
