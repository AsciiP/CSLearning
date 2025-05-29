/*
  @pintia psid=1920807589209583616 pid=1920807589230555140 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part1)-虚函数
  Title: 2017Final进位与借位
  https://pintia.cn/problem-sets/1920807589209583616/exam/problems/type/7?problemSetProblemId=1920807589230555140
*/
// @pintia code=start
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Group
{
public:
    virtual int add(int x, int y) = 0;
    virtual int sub(int x, int y) = 0;
    virtual ~Group() {}
};

class GroupA : public Group
{
public:
    int add(int x, int y) override
    {
        return x + y;
    }
    int sub(int x, int y) override
    {
        return x - y;
    }
};

class GroupB : public Group
{
public:
    int add(int x, int y) override
    {
        return x + y;
    }
    int sub(int x, int y) override
    {
        // 忘记借位
        int res = 0, base = 1;
        while (x > 0 || y > 0)
        {
            int a = x % 10, b = y % 10;
            int d = a - b;
            if (d < 0)
                d += 10; // 不借位
            res += d * base;
            x /= 10;
            y /= 10;
            base *= 10;
        }
        return res;
    }
};

class GroupC : public Group
{
public:
    int add(int x, int y) override
    {
        // 忘记进位
        int res = 0, base = 1;
        while (x > 0 || y > 0)
        {
            int a = x % 10, b = y % 10;
            int d = (a + b) % 10;
            res += d * base;
            x /= 10;
            y /= 10;
            base *= 10;
        }
        return res;
    }
    int sub(int x, int y) override
    {
        // 忘记借位
        int res = 0, base = 1;
        while (x > 0 || y > 0)
        {
            int a = x % 10, b = y % 10;
            int d = a - b;
            if (d < 0)
                d += 10; // 不借位
            res += d * base;
            x /= 10;
            y /= 10;
            base *= 10;
        }
        return res;
    }
};

int main()
{
    int n;
    cin >> n;
    vector<Group *> students(n);
    for (int i = 0; i < n; ++i)
    {
        int t;
        cin >> t;
        if (t == 1)
            students[i] = new GroupA();
        else if (t == 2)
            students[i] = new GroupB();
        else
            students[i] = new GroupC();
    }
    while (true)
    {
        int idx;
        string expr;
        cin >> idx;
        if (idx == 0)
            break;
        cin >> expr;
        string::size_type pos = expr.find('+');
        if (pos != string::npos)
        {
            int x = stoi(expr.substr(0, pos));
            int y = stoi(expr.substr(pos + 1));
            cout << students[idx - 1]->add(x, y) << endl;
        }
        else
        {
            pos = expr.find('-');
            int x = stoi(expr.substr(0, pos));
            int y = stoi(expr.substr(pos + 1));
            cout << students[idx - 1]->sub(x, y) << endl;
        }
    }
    for (auto p : students)
        delete p;
    return 0;
}
// @pintia code=end