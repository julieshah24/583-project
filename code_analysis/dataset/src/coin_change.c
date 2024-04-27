#include <stdio.h>
#include <stdlib.h>


int coinChange(int* coins, int coinsSize, int amount)
{
    int i, j;
    int *dp = malloc((amount + 1) * sizeof(int));

    /* The dp array records minimum coin number corresponding to the
     * amount of coins. So we need to initialize each dp[i] with
     * amount + 1 as an invalid value */
    dp[0] = 0;
    for (i = 1; i <= amount; i++) {
        /* initialized with INT_MAX */
        dp[i] = amount + 1;
        for (j = 0; j < coinsSize; j++) {
            if (i - coins[j] >= 0) {
                int tmp = 1 + dp[i - coins[j]];
                dp[i] = tmp < dp[i] ? tmp : dp[i];
            }
        }
    }

    return dp[amount] == amount + 1 ? -1 : dp[amount];
}

int main(int argc, char **argv)
{
    int coins1[3] = {1,2,5};
    printf("%d\n", coinChange(coins1, 3, 11));
    int coins2[1] = {2};
    printf("%d\n", coinChange(coins2, 1, 3));
    int coins3[1] = {1};
    printf("%d\n", coinChange(coins3, 1, 0));
    return 0;
}
