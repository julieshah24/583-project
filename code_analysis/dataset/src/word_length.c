#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lengthOfLastWord(char *s)
{
    int word_len = 0;
    int len = strlen(s);

    while (len > 0 && s[--len] == ' ') {}

    while (len >= 0 && s[len] != ' ') {
        word_len++;
        len--;
    }

    return word_len;
}

int main()
{
    printf("%d\n", lengthOfLastWord("abcdefghijklmnopqrstuvwxyz"));
    return 0;
}
