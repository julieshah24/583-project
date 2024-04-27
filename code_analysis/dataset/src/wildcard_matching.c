#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


static bool isMatch(char* s, char* p)
{
    char *last_s = NULL;
    char *last_p = NULL;
    while (*s != '\0') {
        if (*p == '*') {
            /* skip the '*', and mark a flag */
            if (*++p == '\0') {
                return true;
            }
            /* use last_s and last_p to store where the "*" match starts. */
            last_s = s;
            last_p = p;
        } else if (*p == '?' || *s == *p) {
            s++;
            p++;
        } else if (last_s != NULL) {
            /* check "last_s" to know whether meet "*" before
             * if meet "*" previously, and the *s != *p
             * reset the p, using '*' to match this situation
             */
            p = last_p;
            s = ++last_s;
        } else {
            /* *p is not wildcard char, 
             * doesn't match *s, 
             * there are no '*' wildcard matched before
             */
            return false;
        }
    }

    /* s is done, but "p" still have chars. */
    while (*p == '*') {
        p++;
    }
    return *p == '\0';
}

int main(int argc, char **argv)
{
    char *str1 = "aa\n";
    char *str2 = "a\n";
    printf("%s\n", isMatch(str1, str2) ? "true" : "false");
    char *str3 = "aa\n";
    char *str4 = "*\n";
    printf("%s\n", isMatch(str3, str4) ? "true" : "false");
    char *str5 = "cb\n";
    char *str6 = "?a\n";
    printf("%s\n", isMatch(str5, str6) ? "true" : "false");
    return 0;
}
