#include <stdio.h>
#include <stdlib.h>

static int rangeBitwiseAnd(int m, int n)
{
    int i, res = 0;
    for (i = 0; m > 0 && n > 0; i++) {
        if (m == n && (m & 1)) {
            res |= 1 << i;
        }

        m = m >> 1;
        n = n >> 1;
    }

    return res;
}

int main(int argc, char **argv)
{
    printf("%d\n", rangeBitwiseAnd(5, 7));
    printf("%d\n", rangeBitwiseAnd(0, 0));
    printf("%d\n", rangeBitwiseAnd(1, 2147483647));
    return 0;
}
