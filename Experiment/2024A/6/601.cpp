/*
 * @Author: 2405111B08 - 王子睿
 * @Date: 2024-11-26 18:02:14
 * @Description: 输入n个字符串，将他们按字母字典序从小到大顺序输出。
 */
#include <iostream>
#include <string>

using namespace std;

int main() {
    int n;
    cin >> n;
    string str[n];
    for (int i = 0; i < n; i++) {
        cin >> str[i];
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (str[i] > str[j]) {
                string temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        cout << str[i] << endl;
    }
    return 0;
}
