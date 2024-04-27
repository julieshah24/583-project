#include <stdio.h>
#include <stdlib.h>

static int trailingZeroes(int n)
{
    /* As 10 = 2 * 5 so we just count how many fives in it */
    return n == 0 ? 0 : n / 5 + trailingZeroes(n / 5);
}

int main()
{
    printf("%d\n", trailingZeroes(100200));
    return 0;
}
