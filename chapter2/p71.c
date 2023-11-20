#include "test.h"

typedef unsigned packed_t;

int xbyte(packed_t word, int bytenum)
{
    int max_bytes = 3;
    int shifted_left = (int)(word << ((max_bytes - bytenum) << 3));
    int result = (shifted_left >> (max_bytes << 3));

    return result;
}

/* The problem with the example is that the function does not work with negative numbers because 
   right shifting an unsigned number does not replicate the sign bit. Additionally, with this version 
   the last "& 0xFF" has to be removed, or it will trash out the leading 1's (including the sign bit). */
int main()
{
    assert(xbyte(0x00112233, 0) == 0x33);
    assert(xbyte(0x00112233, 1) == 0x22);
    assert(xbyte(0x00112233, 2) == 0x11);
    assert(xbyte(0x00112233, 3) == 0x00);

    assert(xbyte(0xAABBCCDD, 0) == 0xFFFFFFDD);
    assert(xbyte(0xAABBCCDD, 1) == 0xFFFFFFCC);
    assert(xbyte(0xAABBCCDD, 2) == 0xFFFFFFBB);
    assert(xbyte(0xAABBCCDD, 3) == 0xFFFFFFAA);

    printf(GRN "Tests passed\n" RESET);
}
