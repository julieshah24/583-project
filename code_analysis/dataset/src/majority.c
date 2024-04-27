#include <stdio.h>
#include <stdlib.h>

static int majorityElement(int* nums, int numsSize)
{
    int i, major, count = 0;
    for (i = 0; i < numsSize; i++) {
        if (count == 0) {
            major = nums[i];
            count++;
        } else if (major == nums[i]) {
            count++;
        } else {
            count--;
        }
    }
    return major;
}

int main(int argc, char **argv)
{
    int nums[3] = {3,2,3};
    printf("%d\n", majorityElement(nums, 3));
     int nums1[7] = {2,2,1,1,1,2,2};
    printf("%d\n", majorityElement(nums1, 7));
    return 0;
}
