/*
  @pintia psid=1903955893807046656 pid=1903955893832212491 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 复数类的操作
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212491
*/
// @pintia code=start
#include <iostream>

class Complex
{
private:
    double real, imag;
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    Complex opposite() const
    {
        return Complex(-real, -imag);
    }
    void print() const
    {
        std::cout << '(' << real << ", " << imag << ')' << std::endl;
    }
    Complex operator+(const Complex &c) const
    {
        return Complex(real + c.real, imag + c.imag);
    }
};

int main()
{
    double r1, i1, r2, i2;
    std::cin >> r1 >> i1 >> r2 >> i2;
    Complex c1(r1, i1), c2(r2, i2);
    Complex c3 = c1 + c2;
    c3.print();
    Complex c4 = c2.opposite() + c1;
    c4.print();
    c2.print();
}
// @pintia code=end