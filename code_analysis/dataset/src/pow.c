#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


double fast_pow(double x, int n)
{
    if (n == 0) { return 1.0; }
    if (n == 1) { return x; }
    double t = fast_pow(x, n / 2);
    return n & 1 ? t * t * x : t * t;
}

double my_pow(double x, int n)
{
    if (n == INT_MIN) {
        double t = 1 / fast_pow(x, -(n / 2));
        return t * t;
    }
    return n < 0 ? 1 / fast_pow(x, -n) : fast_pow(x, n);
}

int main()
{
    printf("%lf\n", my_pow(7, 8));
    return 0;
}
