#ifndef _OPEN_DSP_WAVFILEREADER_H_
#define _OPEN_DSP_WAVFILEREADER_H_

#include <OpenDsp/OpenDsp.hpp>
#include <OpenDsp/Signal.hpp>
#include <OpenDsp/FrameIO/FrameReader.hpp>

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

#include <OpenDsp/FrameIO/WavFileFormat.hpp>

namespace opendsp
{
template<typename SampleType>
class WavFileReader : public FrameReader<SampleType>
{
public:
    WavFileReader(const char* path);
    ~WavFileReader() override;
    uint32_t getSamplingRate() const;
    void open() override;
    void close() override;
    bool hasMoreFrames() const override;
    void getNextFrame(Signal<SampleType>& dst) override;
    uint16_t getChannels() const;
    uint32_t getSamplesPerSec() const;
    uint16_t getBitsPerSample() const;

private:
    std::string path;
    std::ifstream file;
    RiffSection rs;
    FormatSection fs;
    DataSection ds;
};
}

template<typename SampleType>
opendsp::WavFileReader<SampleType>::WavFileReader(const char* path) :
    path{path}
{
    ;
}

template<typename SampleType>
opendsp::WavFileReader<SampleType>::~WavFileReader()
{
    close();
}

template<typename SampleType>
void opendsp::WavFileReader<SampleType>::open()
{
    file.open(path, std::ios::binary);

    if(file.bad())
    {
        throw std::runtime_error("Cannot open " + path);
        file.close();
        return;
    }

    //riff section
    file.read((char*) &rs, sizeof(RiffSection));
    if(file.eof())
    {
        throw std::runtime_error("Unexpected enof of file");
        file.close();
        return;
    }
    if(strncmp(rs.chunkId, RIFF_CHUNK_ID, sizeof(RIFF_CHUNK_ID)-1) != 0)
    {
        throw std::runtime_error("Invalid file format: chunk id != riff");
        file.close();
        return;
    }
    if(strncmp(rs.riffType, RIFF_TYPE_WAVE, sizeof(RIFF_TYPE_WAVE)-1) != 0)
    {
        throw std::runtime_error("Invalid file fomrat: type id != wave");
        file.close();
        return;
    }

    //format section
    file.read((char*) &fs, sizeof(FormatSection));
    if(file.eof())
    {
        throw std::runtime_error("Unexpected enof of file");
        file.close();
        return;
    }
    if(strncmp(fs.chunkId, FORMAT_CHUNK_ID, sizeof(FORMAT_CHUNK_ID)-1) != 0)
    {
        throw std::runtime_error("invalid file format: chunk id !=format");
        file.close();
        return;
    }
    if(fs.bitsPerSample % 8 != 0)
    {
        throw std::runtime_error("Unspported bits per samples: must be a multiple of 8");
        file.close();
        return;
    }
    OPEN_DSP_COND_WARNING(fs.bitsPerSample > 8*sizeof(SampleType), "sample type might be too small");

    //Data Section
    file.read((char*) &ds, sizeof(DataSection));
    if(file.eof())
    {
        throw std::runtime_error("Unexpected enof of file");
        file.close();
        return;
    }
    if(strncmp(ds.chunkId, DATA_CHUNK_ID, sizeof(DATA_CHUNK_ID)-1) != 0)
    {
        throw std::runtime_error("Invalid file format: chunk id !=data");
        file.close();
        return;
    }
}



template<typename SampleType>
bool opendsp::WavFileReader<SampleType>::hasMoreFrames() const
{
    return file.is_open();
}

template<typename SampleType>
void opendsp::WavFileReader<SampleType>::getNextFrame(opendsp::Signal<SampleType>& dst)
{
    if(!hasMoreFrames())
    {
        return;
    }
    int i=0;
    for(i=0; i<dst.getLength(); i++)
    {
        //TODO read multiple samples
        switch(fs.bitsPerSample)
        {
        case 8:
        {
            int8_t s;
            file.read((char*)&s, 1);
            dst[i] = s;
            break;
        }
        case 16:
        {
            int16_t s;
            file.read((char*)&s, 2);
            dst[i] = s;
            break;
        }
        case 32:
        {
            int8_t s;
            file.read((char*)&s, 4);
            dst[i] = s;
            break;
        }
        }
        if(file.eof())
        {
            break;
        }
    }
    if(file.eof())
    {
        file.close();
        //if we reached the eof before frame is full, fill remaining with zeros
        for(; i<dst.getLength(); i++)
        {
            dst[i] = 0;
        }
    }
}

template<typename SampleType>
void opendsp::WavFileReader<SampleType>::close()
{
    file.close();
    if(file.bad())
    {
        throw std::runtime_error("IO error");
    }
}

template<typename SampleType>
uint16_t opendsp::WavFileReader<SampleType>::getChannels() const
{
    return fs.channels;
}

template<typename SampleType>
uint32_t opendsp::WavFileReader<SampleType>::getSamplesPerSec() const
{
    return fs.samplesPerSec;
}

template<typename SampleType>
uint16_t opendsp::WavFileReader<SampleType>::getBitsPerSample() const
{
    return fs.bitsPerSample;
}


#endif
