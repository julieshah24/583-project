#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int lengthOfLongestSubstring(char *s)
{
    int count[256] = {0};
    int len = 0;
    int i, j;

    for (i = 0, j = 0; s[i] != '\0'; i++) {
        count[s[i]]++;
        while (count[s[i]] > 1) {
            len = i - j > len ? i - j : len;
            count[s[j++]] -= 1;
        }
    }

    return i - j > len ? i - j : len;
}

int main(int argc, char **argv)
{
    
    char *str1 = "abcabcbb\n";
    char *str2 = "bbbbb\n";
    char *str3 = "pwwkew\n";
    printf("%d\n", lengthOfLongestSubstring(str1));
    printf("%d\n", lengthOfLongestSubstring(str2));
    printf("%d\n", lengthOfLongestSubstring(str3));
    return 0;
}
