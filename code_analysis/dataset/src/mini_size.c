#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static inline int min(int a, int b)
{
    return a < b ? a : b;
}

static int minSubArrayLen(int s, int* nums, int numsSize)
{
    int i, sum = 0, len = INT_MAX, start = 0;
    for (i = 0; i < numsSize; i++) {
        sum += nums[i];
        while (sum >= s) {
            /* sliding window */
            len = min(len, i - start + 1);
            sum -= nums[start++];
        }
    }

    return len == INT_MAX ? 0 : len;
}

int main(int argc, char **argv)
{

    int count = 6;
    int sum = 7;
    int nums[6] = {2,3,1,2,4,3};
    printf("%d\n", minSubArrayLen(sum, nums, count));
    count = 3;
    sum = 4;
    int nums1[3] = {1,4,4};
    printf("%d\n", minSubArrayLen(sum, nums1, count));
    count = 8;
    sum = 11;
    int nums2[8] = {1,1,1,1,1,1,1,1};
    printf("%d\n", minSubArrayLen(sum, nums2, count));
    return 0;
}
