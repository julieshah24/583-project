#include <stdio.h>
#include <stdlib.h>

static inline void swap(int *a, int *b)
{
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static void reverse(int *a, int size)
{
    int lo = 0;
    int hi = size - 1;
    while (lo < hi) {
        swap(a + lo, a + hi);
        lo++;
        hi--;
    }
}

static void nextPermutation(int* nums, int numsSize)
{
    if (numsSize <= 1) {
        return;
    }

    int i = numsSize - 2;
    while (i >= 0 && nums[i] >= nums[i + 1]) {
        i--;
    }

    if (i >= 0) {
        int j = numsSize - 1;
        while (j >= 0 && nums[j] <= nums[i]) {
            j--;
        }
        swap(nums + i, nums + j);
    }
    reverse(nums + i + 1, numsSize - i - 1);
}

int main(int argc, char **argv)
{
    int nums[3] = {1,2,3};
    nextPermutation(nums, 3);

    for (int i = 0; i < 3; i++) {
        printf("%d ", nums[i]);
    }
    putchar('\n');

    int nums1[3] = {3,2,1};
    nextPermutation(nums1, 3);

    for (int i = 0; i < 3; i++) {
        printf("%d ", nums1[i]);
    }
    putchar('\n');

    int nums2[3] = {1,1,5};
    nextPermutation(nums2, 3);

    for (int i = 0; i < 3; i++) {
        printf("%d ", nums2[i]);
    }
    putchar('\n');

    return 0;
}
