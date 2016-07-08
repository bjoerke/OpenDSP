#ifndef _OPEN_DSP_WAV_FILE_FORMAT_H_
#define _OPEN_DSP_WAV_FILE_FORMAT_H_

const char RIFF_CHUNK_ID[] = "RIFF";
const char FORMAT_CHUNK_ID[] = "fmt ";
const char DATA_CHUNK_ID[] = "data";
const char RIFF_TYPE_WAVE[] = "WAVE";
const uint16_t FORMAT_TAG_PCM = 0x0001;

typedef struct RiffSection
{
    char chunkId[4];
    uint32_t chunkSize; //filesize (bytes) - 8
    char riffType[4];
} __attribute__ ((packed)) RiffSection;

typedef struct FormatSection
{
    char chunkId[4];
    uint32_t length;
    uint16_t formatTag;
    uint16_t channels;
    uint32_t samplesPerSec;
    uint32_t avgBytesPerSec;
    uint16_t blockAlign;  //frame size
    uint16_t bitsPerSample; //PCM only
} __attribute__((packed)) FormatSection;

typedef struct DataSection
{
    char chunkId[4];
    uint32_t length;
    //samples follow
} __attribute__((packed)) DataSection;

#endif
