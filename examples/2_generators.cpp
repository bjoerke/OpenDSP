#include <iostream>

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <OpenDsp/SignalGenerators/SineGenerator.hpp>
#include <OpenDsp/FrameIO/WavFileWriter.hpp>

typedef float SampleType;

using namespace opendsp;
using namespace std;

int main (int argc, char *argv[])
{
    Signal<SampleType> frame(1024);
    WavFileWriter<SampleType> writer("sine.wav");
    SineGenerator<SampleType> generator(2,1);

    writer.open();
    for(int i=0; i<200; i++)
    {
        generator.generateNextFrame(frame);
        writer.writeFrame(frame);
    }
    writer.close();
}
