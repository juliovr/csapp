#include "test.h"

#define INT_MAX 0x7FFFFFFF
#define INT_MIN 0x80000000

/* Addition that saturates to TMin or TMax */
int saturating_add(int x, int y)
{
    int sum = x + y;
    int w = sizeof(int) << 3;

    // Determine which overflow ocurrs
    int pos_overflow = ~(x & INT_MIN) && ~(y & INT_MIN) && (sum & INT_MIN);
    int neg_overflow = (x & INT_MIN) && (y & INT_MIN) && ~(sum & INT_MIN);

    // Mask for each overflow
    int pos_mask = ~(pos_overflow - 1) & INT_MAX;
    int neg_mask = ~(neg_overflow - 1) & INT_MIN;

    int apply_mask = ~(pos_overflow - 1) | ~(neg_overflow - 1); // Should mask be applied?
    int result = (~apply_mask & sum) + (apply_mask & (pos_mask | neg_mask)); // Use sum or mask deppending if the mask should be applied

    return result;
}

int main()
{
    assert(saturating_add(INT_MAX, 0x1234) == INT_MAX);
    assert(saturating_add(INT_MIN, -0x1234) == INT_MIN);
    assert(saturating_add(0x11, 0x22) == 0x11 + 0x22);
    
    print_test_ok("Tests passed\n");

    return 0;
}
