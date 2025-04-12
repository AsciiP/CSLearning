/*
 * @Author: 2405111B08 - 王子睿
 * @Date: 2024-11-29 21:30:18
 * @Description: 查验身份证
 */
#include <iostream>
using namespace std;

int main(){
    int amount;
    int weights[17] = {7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2};
    int Ms[11] = {1, 0, -1, 9, 8, 7, 6, 5, 4, 3, 2};
    
    cin >> amount;
    getchar();
    bool is_allPassed = true;
    for (int t = 0; t < amount; ++ t)
    {
        char id[20];
        int sum = 0;
        for (int i = 0; i < 17; ++ i)
        {
            char each = getchar();
            id[i] = each;
            int digit = each - '0';
            sum += digit * weights[i];
        }
        char last = getchar();
        int Z = sum % 11;
        int M = Ms[Z];
        int A;
        if (last == 'X') A = -1;
        else A = last - '0';
        if (A == M) 
            // cout << "Pass!" << id << last;
            ;
        else 
        {
            // cout << "No!" << id << last;
            cout << id << last << endl;
            is_allPassed = false;
        }
        getchar();
    }
    if (is_allPassed)
        cout << "All passed" << endl;
    return 0;
}