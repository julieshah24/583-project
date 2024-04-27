#include <stdio.h>
#include <stdlib.h>


int removeElement(int *nums, int numsSize, int val)
{
    int i, count = 0;
    for (i = 0; i < numsSize; i++) {
        if (nums[i] != val) {
            nums[count++] = nums[i];
        }
    }
    return count;
}

int main(int argc, char **argv)
{
    int i;
    int arg_c = 6;
    int *nums = malloc((arg_c - 2) * sizeof(int));
    for (i = 0; i < argc - 2; i++) {
        nums[i] = i;
    }

    int count = removeElement(nums, arg_c - 2, 2);
    for (i = 0; i < count; i++) {
        printf("%d ", nums[i]);
    }
    printf("\n");

    return 0;
}
