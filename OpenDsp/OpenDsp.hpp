#ifndef _OPEN_DSP_H_
#define _OPEN_DSP_H_

#include <OpenDsp/Math/Complex.hpp>
#include <stdbool.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef opendsp::Complex<float> cfloat;
typedef opendsp::Complex<double> cdouble;

#ifdef OPEN_DSP_DEBUG

#include <iostream>

#define OPEN_DSP_WARNING(message)            _open_dsp_warning(message, __LINE__, __FILE__)
#define OPEN_DSP_COND_WARNING(cond, message) _open_dsp_cond_warning(cond, message, __LINE__, __FILE__);

void _open_dsp_warning(const char* message, int line, const char* file)
{
    std::cout << "[WARNING]: " << message << " @  " << file << "(" << line << ")" << std::endl;
}

void _open_dsp_cond_warning(bool condition, const char* message, int line, const char* file)
{
    if(condition) _open_dsp_warning(message, line, file);
}

#else

#define OPEN_DSP_WARNING(message)            ;
#define OPEN_DSP_COND_WARNING(cond, message) ;


#endif //OPEN_DSP_DEBUG

bool isPowerOfTwo(uint v)
{
    //from http://graphics.stanford.edu/~seander/bithacks.html#DetermineIfPowerOf2
    return (v && !(v & (v - 1)));
}

#endif
