#include <stdlib.h>
#include <stdio.h>

double poly(double a[], double x, long degree)
{
    long i;
    double result = a[0];
    double xpwr = x;
    for (i = 1; i <= degree; ++i) {
        result += a[i] * xpwr;
        xpwr = x * xpwr;
    }

    return result;
}

double poly_5x5(double a[], double x, long degree)
{
    long i;
    double result1 = a[0];
    double result2 = 0;
    double result3 = 0;

    double x_increment = x * x * x * x * x;
    
    double xpwr1 = x;
    double xpwr2 = x * x * x;
    double xpwr3 = x * x * x * x * x;

    for (i = 1; i <= degree - 5; i += 5) {
        result1 += a[i] * xpwr1 + (a[i + 1] * xpwr1 * x);
        result2 += a[i + 2] * xpwr2 + (a[i + 3] * xpwr2 * x);
        result3 += a[i + 4] * xpwr3;

        xpwr1 *= x_increment;
        xpwr2 *= x_increment;
        xpwr3 *= x_increment;
    }

    for (; i <= degree; i++) {
        result1 += a[i] * xpwr1;
        xpwr1 *= x;
    }

    return result1 + result2 + result3;
}

int main()
{
    double a[] = {
        1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0,
    };

    double x = 2;
    long degree = 20;

#if 0
    double result = poly(a, x, degree);
    printf("result = %.2f\n", result);
#else
    double result_loop_unrolling = poly_5x5(a, x, degree);
    printf("result_loop_unrolling = %.2f\n", result_loop_unrolling);
#endif
}
