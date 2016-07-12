#ifndef _OPEN_DSP_WAVEFILEWRITER_H_
#define _OPEN_DSP_WAVEFILEWRITER_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/FrameIO/FrameWriter.hpp>

#include <OpenDsp/FrameIO/WavFileFormat.hpp>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdexcept>
namespace opendsp
{

template<typename SampleType>
class WavFileWriter : public FrameWriter<SampleType>
{
public:
    WavFileWriter(const char* path) ;
    ~WavFileWriter() override;
    void writeFrame(Signal<SampleType>& frame) override;
    void open() override;
    void close() override;
    void setSamplesPerSec(const uint32_t sps);
    void setBitsPerSample(const uint16_t bps);

private:
    std::string path;
    uint16_t bitsPerSample;
    uint32_t samplesPerSec;
    std::ofstream file;
    RiffSection riffSection;
    FormatSection formatSection;
    DataSection dataSection;
};

}

template<typename SampleType>
opendsp::WavFileWriter<SampleType>::WavFileWriter(const char* path) :
    path{path},
    bitsPerSample(16),
    samplesPerSec{41000}
{
    //init RIFF section
    strncpy(riffSection.chunkId, RIFF_CHUNK_ID, sizeof(RIFF_CHUNK_ID)-1);
    strncpy(riffSection.riffType, RIFF_TYPE_WAVE, sizeof(RIFF_TYPE_WAVE)-1);
    //Init FORMAT section
    strncpy(formatSection.chunkId, FORMAT_CHUNK_ID, sizeof(FORMAT_CHUNK_ID)-1);
    formatSection.length = 16;
    formatSection.formatTag = FORMAT_TAG_PCM;
    formatSection.channels = 1;

    //init DATA section
    strncpy(dataSection.chunkId, DATA_CHUNK_ID, sizeof(DATA_CHUNK_ID)-1);
}

template<typename SampleType>
opendsp::WavFileWriter<SampleType>::~WavFileWriter()
{
    close();
}

template<typename SampleType>
void opendsp::WavFileWriter<SampleType>::open()
{
    file.open(path, std::ios::binary);
    file.seekp(sizeof(RiffSection) + sizeof(FormatSection) + sizeof(DataSection));
    if(!file.good())
    {
        throw std::runtime_error("IO error");
    }
}

template<typename SampleType>
void opendsp::WavFileWriter<SampleType>::close()
{
    long pos = file.tellp();
    file.seekp(0);
    riffSection.chunkSize = pos - 8;
    dataSection.length = pos-44;
    formatSection.samplesPerSec = samplesPerSec;
    formatSection.avgBytesPerSec = samplesPerSec * (bitsPerSample / 8);
    formatSection.blockAlign = bitsPerSample / 8;
    formatSection.bitsPerSample = bitsPerSample;

    file.write( (const char*) &riffSection, sizeof(RiffSection) );
    file.write( (const char*) &formatSection, sizeof(FormatSection));
    file.write( (const char*) &dataSection, sizeof(DataSection));

    file.close();
}

template<typename SampleType>
void opendsp::WavFileWriter<SampleType>::writeFrame(Signal<SampleType>& frame)
{
    for(int i=0; i<frame.getLength(); i++)
    {
        SampleType sample = frame[i];
        switch(bitsPerSample)
        {
        case 8:
        {
            int8_t s = sample;
            file.write( (const char*) &s, 1);
            break;
        }
        case 16:
        {
            int16_t s = sample;
            file.write( (const char*) &s, 2);
            break;
        }
        case 32:
        {
            int32_t s = (uint32_t) sample;
            file.write( (const char*) &s, 4);
            break;
        }
        }
    }
}

template<typename SampleType>
void opendsp::WavFileWriter<SampleType>::setSamplesPerSec(uint32_t sps)
{
    if(file.is_open())
    {
        throw std::runtime_error("open() has already been called");
    }
    samplesPerSec = sps;
}

template<typename SampleType>
void opendsp::WavFileWriter<SampleType>::setBitsPerSample(uint16_t bps)
{
    if(file.is_open())
    {
        throw std::runtime_error("open() has already been called");
    }
    OPEN_DSP_COND_WARNING(
        sizeof(SampleType) > bitsPerSample / 8,
        "maybe more bitsPerSample needed to store SampleTpe"
    );
    OPEN_DSP_COND_WARNING(
        bitsPerSample % 8 != 0,
        "bits per sample must be a multiple of 8"
    );
    bitsPerSample = bps;
}


#endif
