#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static int max(int a, int b)
{
    return a > b ? a : b;
}

int longestCommonSubsequence(char * text1, char * text2)
{
    int i, j;
    int l1 = strlen(text1);
    int l2 = strlen(text2);
    int **dp = malloc((l1 + 1) * sizeof(int *));
    for (i = 0; i < l1 + 1; i++) {
        dp[i] = malloc((l2 + 1) * sizeof(int));
    }
    memset(dp[0], 0, (l2 + 1) * sizeof(int));
    for (i = 1; i <= l1; i++) {
        dp[i][0] = 0;
    }
    
    for (i = 1; i <= l1; i++) {
        for (j = 1; j <= l2; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[l1][l2];
}

int main(int argc, char **argv)
{
    char *str1 = "abcde\n";
    char *str2 = "ace\n";
    printf("%d\n", longestCommonSubsequence(str1, str2));
    char *str3 = "abc\n";
    char *str4 = "abc\n";
    printf("%d\n", longestCommonSubsequence(str3, str4));
    char *str5 = "abc\n";
    char *str6 = "def\n";
    printf("%d\n", longestCommonSubsequence(str5, str6));
    return 0;
}
