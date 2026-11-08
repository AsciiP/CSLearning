#include <iostream>
#include <vector>
#include <algorithm> // For std::max

const int NUM_STORES = 3;
const int NUM_EMPLOYEES = 5;

const int p[4][6] = {
    {0, 0, 0, 0, 0, 0},
    {0, 3, 7, 9, 12, 13},
    {0, 5, 10, 11, 11, 11},
    {0, 4, 6, 11, 12, 12}
};


int dp[4][6];
int path[4][6];

void solve()
{
    for (int j = 0; j <= NUM_EMPLOYEES; ++j)
    {
        dp[3][j] = p[3][j];
        path[3][j] = j;
    }


    for (int i = 2; i >= 1; --i)
    {
        for (int j = 0; j <= NUM_EMPLOYEES; ++j)
        {
            int max_profit = -1;
            int best_k = 0;
            for (int k = 0; k <= j; ++k)
            {
                int current_profit = p[i][k] + dp[i + 1][j - k];

                if (current_profit > max_profit)
                {
                    max_profit = current_profit;
                    best_k = k;
                }
            }
            dp[i][j] = max_profit;
            path[i][j] = best_k;
        }
    }

    std::cout << "最大总赢利为: " << dp[1][NUM_EMPLOYEES] << " 万元" << std::endl;

    int employees_remaining = NUM_EMPLOYEES;
    std::cout << "最优分配方案:" << std::endl;
    for (int i = 1; i <= NUM_STORES; ++i)
    {
        int assigned = path[i][employees_remaining];
        char store_name = 'A' + (i - 1);
        std::cout << "  商店 " << store_name << ": " << assigned << " 人" << std::endl;
        employees_remaining -= assigned;
    }
}

int main()
{
    solve();
    return 0;
}