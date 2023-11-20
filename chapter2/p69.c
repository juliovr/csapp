#include "test.h"

unsigned rotate_left(unsigned x, int n)
{
    unsigned w = sizeof(unsigned) << 3;
    unsigned rotated_value = x >> (w - n);
    unsigned mask = -1 << n;
    x = x << n;
    x = x & mask;
    x = x | rotated_value;

    return x;
}

int main()
{
    assert(rotate_left(0x12345678, 4) == 0x23456781);
    assert(rotate_left(0x12345678, 20) == 0x67812345);

    printf(GRN "Tests passed\n" RESET);
}
