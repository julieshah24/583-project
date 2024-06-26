#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


struct palindrome {
    int low;
    int high;
};

static void collect(char *s, int len, int low, int high, struct palindrome *results, int *count)
{
    while (low >= 0 && high < len && s[low] == s[high]) {
        results[*count].low = low;
        results[*count].high = high;
        (*count)++;
        low--;
        high++;
    }
}

static void dfs(struct palindrome *pal_set, int num, int start,
                char *s, int len, struct palindrome **stack, int size,
                char ***results, int *count, int *col_sizes)
{
    int i;
    if (size > 0 && stack[size - 1]->high == len - 1) {
        col_sizes[*count] = size;
        results[*count] = malloc(size * sizeof(char *));
        for (i = 0; i < size; i++) {
            int low = stack[i]->low;
            int high = stack[i]->high;
            results[*count][i] = malloc(high - low + 2);
            memcpy(results[*count][i], s + low, high - low + 1);
            results[*count][i][high - low + 1] = '\0';
        }
        (*count)++;
    } else {
        for (i = start; i < num; i++) {
            if ((size == 0 && pal_set[i].low == 0) ||
                (size > 0 && stack[size - 1]->high + 1 == pal_set[i].low)) {
                stack[size] = &pal_set[i];
                dfs(pal_set, num, i + 1, s, len, stack, size + 1, results, count, col_sizes);
            }
        }
    }
}

/**
 ** Return an array of arrays of size *returnSize.
 ** The sizes of the arrays are returned as *returnColumnSizes array.
 ** Note: Both returned array and *returnColumnSizes array must be malloced, assume caller calls free().
 **/
char ***partition(char* s, int* returnSize, int** returnColumnSizes)
{
    int len = strlen(s);
    if (len == 0) {
        *returnSize = 0;
        return NULL;
    }

    int i, cap = 800, count = 0;
    struct palindrome *pal_set = malloc(cap * sizeof(*pal_set));
    for (i = 0; i < len; i++) {
        collect(s, len, i, i, pal_set, &count);
        collect(s, len, i, i + 1, pal_set, &count);
    }

    char ***results = malloc(cap * sizeof(char **));
    struct palindrome **stack = malloc(count * sizeof(*stack));
    *returnColumnSizes = malloc(cap * sizeof(int));
    dfs(pal_set, count, 0, s, len, stack, 0, results, returnSize, *returnColumnSizes);
    return results;
}

int main()
{
    int i, j, count = 0;
    int *col_sizes;
    char ***lists = partition("amiapalindromeemordnilapaima", &count, &col_sizes);
    for (i = 0; i < count; i++) {
        char **list = lists[i];
        for (j = 0; j < col_sizes[i]; j++) {
            printf("%s ", list[j]);
        }
        printf("\n");
    }
    return 0;
}
