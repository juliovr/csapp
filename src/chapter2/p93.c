#include "test.h"

#include <math.h>

/* Acess bit-level representation floating-point number */
typedef unsigned float_bits;

/* Compute -f. If f is Nan, then return f. */
float_bits float_absval(float_bits f)
{
    unsigned exp = f >> 23 & 0xFF;
    unsigned frac = f & 0x7FFFFF;

    if (exp == 0xFF && frac != 0) {
        return f;
    }

    return (0 << 31) | (exp << 23) | frac;
}

float u2f(unsigned x)
{
	return *(float *) &x;
}

unsigned f2u(float f)
{
	return *(unsigned *) &f;
}

int main()
{
    float f;
    unsigned uf;

    f = -0.0;
    uf = f2u(f);
    assert(abs(f) == (u2f(float_absval(uf))));

    f = 0.0;
    uf = f2u(f);
    assert(abs(f) == abs(u2f(float_absval(uf))));

    f = 1.0;
    uf = f2u(f);
    assert(abs(f) == abs(u2f(float_absval(uf))));

    f = -1.0;
    uf = f2u(f);
    assert(abs(f) == abs(u2f(float_absval(uf))));

    f = u2f(0x7F800000);    /* +oo */
    uf = f2u(f);
    assert(abs(f) == abs(u2f(float_absval(uf))));

    f = u2f(0xFF800000);    /* -oo */
    uf = f2u(f);
    assert(abs(f) == abs(u2f(float_absval(uf))));

    f = u2f(0x7F800001);    /* NaN */
    uf = f2u(f);
    assert(isnan(f) && isnan(u2f(float_absval(uf))));
    
    print_test_ok("Tests passed\n");
}
