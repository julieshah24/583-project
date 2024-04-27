#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void dfs(int *nums, int size, int start, int target, int *stack,
                int len, int **results, int *count, int *column_sizes)
{
    int i;
    if (target < 0) {
        return;
    } else if (target == 0) {
        results[*count] = malloc(len * sizeof(int));
        memcpy(results[*count], stack, len * sizeof(int));
        column_sizes[*count] = len;
        (*count)++;
    } else {
        for (i = start; i < size; i++) {
            stack[len] = nums[i];
            /* The elements in solution can be duplicate for the purpose of the problem */
            dfs(nums, size, i, target - nums[i], stack, len + 1, results, count, column_sizes);
        }
    }
}

/**
 ** Return an array of arrays of size *returnSize.
 ** The sizes of the arrays are returned as *returnColumnSizes array.
 ** Note: Both returned array and *returnColumnSizes array must be malloced, assume caller calls free().
 **/
int** combinationSum(int* candidates, int candidatesSize, int target, int* returnSize, int **returnColumnSizes)
{
    int cap = 200;
    int *stack = malloc(cap * sizeof(int));
    int **results = malloc(cap * sizeof(int *));
    *returnColumnSizes = malloc(cap * sizeof(int));
    *returnSize = 0;
    dfs(candidates, candidatesSize, 0, target, stack, 0, results, returnSize, *returnColumnSizes);
    return results;
}

int main(int argc, char **argv)
{

    int i, j, count = 0;
    int target = 7;
    int nums[4] = {2,3,6,7};

    int *sizes;
    int **lists = combinationSum(nums, 4, target, &count, &sizes);
    for (i = 0; i < count; i++) {
        for (j = 0; j < sizes[i]; j++) {
            printf("%d ", lists[i][j]);
        }
        printf("\n");
    }

    count = 0;
    target = 8;
    int nums2[3] = {2,3,5};

    int *sizes2;
    int **lists2 = combinationSum(nums2, 3, target, &count, &sizes2);
    for (i = 0; i < count; i++) {
        for (j = 0; j < sizes2[i]; j++) {
            printf("%d ", lists2[i][j]);
        }
        printf("\n");
    }
    return 0;
}
