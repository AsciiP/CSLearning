/*
  @pintia psid=1910112809178632192 pid=1910112809203798020 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: A是A1的虚基类
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798020
*/
// @pintia code=start
#include <iostream>
using namespace std;

class A
{
protected:
    int i;

public:
    A(int val) : i(val)
    {
        cout << "Call A:i=" << i << endl;
    }
    void display()
    {
        cout << "i=" << i << endl;
    }
};

class A1 : virtual public A
{
protected:
    int j;

public:
    A1(int val1, int val2) : A(val1), j(val2)
    {
        cout << "Call A1:i=" << i << endl;
    }
};

class A2 : virtual public A
{
protected:
    int k;

public:
    A2(int val1, int val3) : A(val1), k(val3)
    {
        cout << "Call A2:i=" << i << endl;
    }
};

class A3 : public A1, public A2
{
public:
    A3(int val1, int val2, int val3) : A(val1), A1(val1, val2), A2(val1, val3)
    {
        cout << "Call A3:i=" << i << endl;
    }
    void disp()
    {
        display();
        cout << "j=" << j << endl;
        cout << "k=" << k << endl;
    }
};

int main()
{
    int A, B, C;
    cin >> A >> B >> C;
    A3 obj(A, B, C);
    obj.disp();
    return 0;
}
// @pintia code=end