#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void dfs(int *nums, int size, int start, int *stack,
                int len, int **sets, int *count, int *sizes)
{
    int i;
    sets[*count] = malloc(len * sizeof(int));
    memcpy(sets[*count], stack, len * sizeof(int));
    sizes[*count] = len;
    (*count)++;
    for (i = start; i < size; i++) {
        stack[len] = nums[i];
        dfs(nums, size, i + 1, stack, len + 1, sets, count, sizes);
    }
}

/**
 ** Return an array of arrays of size *returnSize.
 ** The sizes of the arrays are returned as *returnColumnSizes array.
 ** Note: Both returned array and *returnColumnSizes array must be malloced, assume caller calls free().
 **/
int** subsets(int* nums, int numsSize, int* returnSize, int** returnColumnSizes)
{
    int capacity = 5000;
    int **sets = malloc(capacity * sizeof(int *));
    int *stack = malloc(numsSize * sizeof(int));
    *returnColumnSizes = malloc(capacity * sizeof(int));
    *returnSize = 0;
    dfs(nums, numsSize, 0, stack, 0, sets, returnSize, *returnColumnSizes);
    return sets;
}

int main()
{
    int i, j;
    int size = 5;
    int nums[5] = {1,2,3,4,5};

    int *sizes;
    int count;
    int **lists = subsets(nums, size, &count, &sizes);
    for (i = 0; i < count; i++) {
        for (j = 0; j < sizes[i]; j++) {
            printf("%d ", lists[i][j]);
        }
        printf("\n");
    }
    return 0;
}
