/*
  @pintia psid=1913050494754209792 pid=1913050494775181312 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part1)-运算符重载
  Title: 重载大于号运算符，比较复数大小
  https://pintia.cn/problem-sets/1913050494754209792/exam/problems/type/7?problemSetProblemId=1913050494775181312
*/
// @pintia code=start
#include <iostream>
#include <cmath>
using namespace std;

class Complex
{
    int real, imag;

public:
    Complex(int r = 0, int i = 0) : real(r), imag(i) {}
    int mod2() const { return real * real + imag * imag; }
    bool operator>(const Complex &other) const
    {
        return this->mod2() > other.mod2();
    }
    bool operator==(const Complex &other) const
    {
        return this->mod2() == other.mod2();
    }
};

int main()
{
    int r1, i1, r2, i2;
    cin >> r1 >> i1 >> r2 >> i2;
    Complex c1(r1, i1), c2(r2, i2);
    if (c1 > c2)
        cout << 1 << endl;
    else if (c1 == c2)
        cout << 0 << endl;
    else
        cout << -1 << endl;
    return 0;
}
// @pintia code=end