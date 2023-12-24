#include "test.h"

#define INT_MIN 0x80000000

/* Determine whether arguments can be subtracted without overflow */
int tsub_ok(int x, int y)
{
    int sub = x - y;
    int neg_overflow = (x & INT_MIN) && (-y & INT_MIN) && ~(sub & INT_MIN);
    int result = !neg_overflow;
    
    return result;
}

int main()
{
    assert(!tsub_ok(INT_MIN, 1000));
    assert(tsub_ok(INT_MIN, 0));
    assert(tsub_ok(100, 100));

    print_test_ok("Tests passed\n");

    return 0;
}
