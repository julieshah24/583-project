#include <stdio.h>
#include <stdlib.h>


static int search(int* nums, int numsSize, int target)
{
    int lo = 0;
    int hi = numsSize - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        if (nums[mid] == target) {
            return mid;
        }

        /* lo might be mid */
        /* We only need to consider non-rotated sorted array search */
        if (nums[lo] <= nums[mid]) {
            if (nums[lo] <= target && target < nums[mid]) {
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        } else {
            if (nums[mid] < target && target <= nums[hi]) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }

    return -1;
}

int main(int argc, char **argv)
{
    int arg_v[] = {1,4,5,6,2};
    int target = 3;
    int i, count = 3;
    int *nums = malloc(count * sizeof(int));
    int size = 3;

    for (i = 0; i < count; i++) {
        nums[i] = arg_v[i + 2];
    }
    printf("%d\n", search(nums, size, target));
    return 0;
}
