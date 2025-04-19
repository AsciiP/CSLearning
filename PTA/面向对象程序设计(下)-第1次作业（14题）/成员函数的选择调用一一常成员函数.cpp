/*
  @pintia psid=1903955893807046656 pid=1903955893832212488 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 成员函数的选择调用一一常成员函数
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212488
*/
// @pintia code=start
#include <iostream>

class Complex
{
private:
    int real, imag;
public:
    Complex(int r = 0, int i = 0) : real(r), imag(i) {}
    void disp()
    {
        std::cout << '(' << real << ',' << imag << 'i' << ')' << std::endl;
    }
    void disp() const
    {
        std::cout << '[' << real << ',' << imag << 'j' << ']' << std::endl;
    }
};

int main()
{
    int arg1, arg2;
    std::cin >> arg1 >> arg2;
    Complex c1(arg1, arg2);

    std::cin >> arg1 >> arg2;
    const Complex c2(arg1, arg2);

    c1.disp();
    c2.disp();

    return 0;
}
// @pintia code=end