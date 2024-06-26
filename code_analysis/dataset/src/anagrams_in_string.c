#include <stdio.h>
#include <stdlib.h>


/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* findAnagrams(char * s, char * p, int* returnSize)
{
    *returnSize = 0;
    int *res = malloc(11000 * sizeof(int));
    int i, pat_len = 0;
    int count[128] = { 0 };
    int l = 0, r = 0, len = 0;

    for (i = 0; p[i] != '\0'; i++) {
        count[p[i]]++;
    }
    pat_len = i;

    while (s[r] != '\0') {
        if (--count[s[r++]] >= 0) {
            len++;
        }

        while (len >= pat_len) {
            if (r - l == pat_len) {
                res[(*returnSize)++] = l;
            }
            if (++count[s[l++]] > 0) {
                len--;
            }
        }
    }

    return res;
}

int main(int argc, char **argv)
{

    char *t = "abc";
    char *s = "cbaebabacd";
    int i, count;
    int *results = findAnagrams(s, t, &count); 
    for (i = 0; i < count; i++) {
        printf("%d ", results[i]);
    }
    printf("\n");

    char *t2 = "ab";
    char *s2 = "abab";
    int i2, count2;
    int *results2 = findAnagrams(s, t, &count2); 
    for (i2 = 0; i2 < count2; i2++) {
        printf("%d ", results2[i2]);
    }
    printf("\n");

    return 0;
}
