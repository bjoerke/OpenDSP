#ifndef _OPENDSP_COMPLEX_H_
#define _OPENDSP_COMPLEX_H_

#include <math.h>
#include <type_traits>

namespace opendsp
{
template<typename T>
class Complex
{
    //this especially avoids a Complex<Complex<T>>
    static_assert(std::is_arithmetic<T>(), "Template parameter must be an aritthmetic type like int or float.");
    public:
        T real;
        T imag;
        Complex() :
            real{0},
            imag{0}
        {
        }
        Complex(T real, T imag) :
            real{real},
            imag{imag}
        {
        }
        Complex(const Complex<T>& c) :
            real{c.real},
            imag{c.imag}
        {
        }
        ~Complex()
        {
        }
        void operator=(T value)
        {
            real = value;
            imag = 0;
        }
        void operator=(const Complex<T>& complex)
        {
            real = complex.real;
            imag = complex.imag;
        }
        void operator+=(const Complex<T>& add)
        {
            real += add.real;
            imag += add.imag;
        }
        void operator-=(Complex<T>& sub)
        {
            real -= sub.real;
            imag -= sub.imag;
        }
        void operator*=(Complex<T>& mul)
        {
            T realTemp = real;
            real = real * mul.real - imag * mul.imag;
            imag = realTemp * mul.imag + imag * mul.real;
        }
        void operator/=(Complex<T>& div)
        {
            T realTemp = real;
            T norm = (div.real * div.real + div.imag * div.imag);
            real = (real * div.real + imag * div.imag) / norm;
            imag = (imag * div.real - realTemp * div.imag) / norm;
        }
        void operator/=(T div)
        {
            real /= div;
            imag /= div;
        }
        T amplitude()
        {
            return sqrt(real * real + imag * imag);
        }
        T phase()
        {
            return atan2(imag, real);
        }
        void conjugate()
        {
            imag = -imag;
        }
};

}

template<typename T>
std::ostream& operator<<(std::ostream& os, const opendsp::Complex<T>& complex)
{
    os << "(" << complex.real << ", " << complex.imag << ")";
    return os;
}


#endif
