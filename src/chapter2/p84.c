#include "test.h"

/* Returns an unsigned 32-bit number having the same bit representation as its floating-point argument. */
unsigned f2u(float x)
{
    unsigned result = *(unsigned *)&x;

    return result;
}

/* Test whether its first argument is less than or equal to its second. */
int float_le(float x, float y)
{
    unsigned ux = f2u(x);
    unsigned uy = f2u(y);

    /* Get the sign bits */
    unsigned sx = ux >> 31;
    unsigned sy = uy >> 31;
    
    int result = ((ux << 1 == 0) && (uy << 1 == 0) || /* Both zeros (+0 or -0) */
                  (sx > sy) ||                        /* Is ux negative and uy doesn't */
                  (sx && sy && ux >= uy) ||           /* If they are both negative, ux is le than uy if its binary representation is ge */
                  (!sx && !sy && ux <= uy));

    return result;
}

int main()
{
    assert(float_le(-0, +0));
	assert(float_le(+0, -0));
	assert(float_le(0, 3));
	assert(float_le(-4, -0));
	assert(float_le(-4, 4));
    assert(!float_le(3, 0));
    assert(float_le(-3, 3));
    assert(!float_le(3, -3));

    print_test_ok("Tests passed\n");
    return 0;
}
