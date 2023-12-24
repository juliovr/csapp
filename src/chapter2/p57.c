#include <stdio.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len)
{
    int i;
    for (i = 0; i < len; ++i) {
        printf(" %.2x", start[i]);
    }
    printf("\n");
}

void show_short(short x)
{
    show_bytes((byte_pointer)&x, sizeof(short));
}

void show_long(long x)
{
    show_bytes((byte_pointer)&x, sizeof(long));
}

void show_double(double x)
{
    show_bytes((byte_pointer)&x, sizeof(double));
}

//
// Test method
//
void test_show_bytes(int val)
{
    int ival = val;
    short sval = (short)ival;
	long lval = (long)ival;
    double dval = (double)ival;
    printf("short  = ");
    show_short(sval);
    printf("long   = ");
    show_long(lval);
    printf("double = ");
    show_double(dval);
}

int main(void)
{
    test_show_bytes(99345);
    test_show_bytes(123);
    test_show_bytes(45);
    test_show_bytes((1<<32) - 1);
}

