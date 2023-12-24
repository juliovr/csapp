#include "test.h"

int main()
{
    int x = 8;
    assert(((x << 4) + x) == (x * 17));
    assert((x - (x << 3)) == (x * -7));
    assert(((x << 6) - (x << 2)) == (x * 60));
    assert(((x << 4) - (x << 7)) == (x * -112));

    print_test_ok("Tests passed\n");
    return 0;
}
