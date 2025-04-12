/*
 * @Author: 2405111B08 - 王子睿
 * @Date: 2024-11-29 21:20:26
 * @Description: 查验身份证
 */
#include <iostream>
using namespace std;
int main()
{
    string M = "10X98765432";
    int N;
    cin >> N;
    int isLegal;
    for (int i = 0; i < N; i++)
    {
        string id;
        cin >> id;
        for (int a = 0; a <= 16; a++)
        {
            if (id[a] >= '0' && id[a] <= '9')
            {
                isLegal = 1;
            }
            else
            {
                isLegal = 0;
                cout << id << endl;
                break;
            }
        }
        int x;
        x = id[0] * 7 + id[1] * 9 + id[2] * 10 + id[3] * 5 + id[4] * 8 + id[5] * 4 + id[6] * 2 + id[7] * 1 + id[8] * 6 + id[9] * 3 + id[10] * 7 + id[11] * 9 + id[12] * 10 + id[13] * 5 + id[14] * 8 + id[15] * 4 + id[16] * 2;
        int z;
        z = x % 11;
        if (M[z] != id[17])
        {
            isLegal = 0;
            cout << id << endl;
        }
    }

    if (isLegal != 0)
    {
        cout << "All passed";
    }
    return 0;
}