#include "test.h"
#include <math.h>

/* Returns floating-point number having the same bit representation as its unsigned 32-bit argument. */
float u2f(unsigned x)
{
    float result = *(float *)&x;

    return result;
}

float fpwr2(int x)
{
    /* Result exponent and fraction */
    unsigned exp, frac;
    unsigned u;

    if (x < 2-pow(2,7)-23) {
        /* Too small. Return 0.0 */
        exp = 0;
        frac = 0;
    } else if (x < 2-pow(2,7)) {
        /* Denormalized result */
        exp = 0;
        frac = 1 << (unsigned)(x - (2-pow(2,7)-23));
    } else if (x < 255) {
        /* Normalized result */
        exp = x + 127;
        frac = 0;
    } else {
        /* Too big. Return +Infinity */
        exp = 0xFF;
        frac = 0;
    }

    /* Pack exp and frac into 32 bits */
    u = exp << 23 | frac;

    /* Return as float */
    return u2f(u);
}

int main()
{
    assert(fpwr2(0) == powf(2,0));
    assert(fpwr2(100) == powf(2,100));
    float a = fpwr2(-100);
    float b = powf(2,-100);
    assert(a == b);
    assert(fpwr2(10000) == powf(2,10000));
    assert(fpwr2(-10000) == powf(2,-10000));

    print_test_ok("Tests passed\n");
}
