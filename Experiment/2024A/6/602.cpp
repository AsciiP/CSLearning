/*
 * @Author: 2405111B08 - 王子睿
 * @Date: 2024-11-26 18:08:22
 * @Description: 输入一行文字，分析其中的内容，找出文字中包含的字符成分，包括：大写字母、小写字母、空格、数字、其他符号。
 */
#include <iostream>
#include <string>

using namespace std;

int main()
{
    string str;
    getline(cin, str);
    int upper = 0, lower = 0, space = 0, digit = 0, other = 0;
    for (int i = 0; i < str.length(); i++)
    {
        if (isupper(str[i]))
        {
            upper++;
        }
        else if (islower(str[i]))
        {
            lower++;
        }
        else if (isspace(str[i]))
        {
            space++;
        }
        else if (isdigit(str[i]))
        {
            digit++;
        }
        else
        {
            other++;
        }
    }
    cout << "大写字母：" << upper << "个" << endl;
    cout << "小写字母：" << lower << "个" << endl;
    cout << "空格：" << space << "个" << endl;
    cout << "数字：" << digit << "个" << endl;
    cout << "其他符号：" << other << "个" << endl;
    return 0;
}