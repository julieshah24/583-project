#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


bool checkInclusion(char * s1, char * s2)
{
    int i, count[128] = { 0 }, pat_len;
    int l = 0, r = 0, len = 0;

    for (i = 0; s1[i] != '\0'; i++) {
        count[s1[i]]++;
    }
    pat_len = i;

    while (s2[r] != '\0') {
        if (--count[s2[r++]] >= 0) {
            len++;
        }

        while (len >= pat_len) {
            if (r - l == pat_len) {
                return true;
            }
            if (++count[s2[l++]] > 0) {
                len--;
            }
        }
    }

    return false;
}

int main()
{
    char *t = "mmnn";
    char *s = "nnmm";
    printf("%s\n", checkInclusion(t, s) ? "true" : "false");
    return 0;
}
