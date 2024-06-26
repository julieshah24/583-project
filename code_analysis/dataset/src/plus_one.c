#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 ** Return an array of size *returnSize.
 ** Note: The returned array must be malloced, assume caller calls free().
 **/
static int* plusOne(int* digits, int digitsSize, int* returnSize)
{
    int i, j, len = 0, carry = 1;
    int *result = malloc((digitsSize + 1) * sizeof(int));
    for (i = digitsSize - 1; i >= 0 || carry; i--) {
        if(i >= 0){
            int n = digits[i] + carry;
            result[len++] = n % 10;
            carry = n / 10;
        } else {
            // add case like [9]
            result[len++] = 1;
            carry = 0;
        }
    }

    for (i = 0, j = len - 1; i < j; i++, j--) {
        int tmp = result[i];
        result[i] = result[j];
        result[j] = tmp;
    }

    *returnSize = len;
    return result;
}

int main()
{
    int i;
    int count = 5;
    int digits[5] = {1,2,3,4,5};

    int len = 0;
    int *res = plusOne(digits, count, &len);
    for (i = 0; i < len; i++) {
        printf("%c", res[i] + '0');
    }
    printf("\n");
    return 0;
}
