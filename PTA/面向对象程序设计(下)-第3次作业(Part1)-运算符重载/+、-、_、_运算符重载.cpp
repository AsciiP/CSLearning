/*
  @pintia psid=1913050494754209792 pid=1913050494775181316 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part1)-运算符重载
  Title: +、-、*、/运算符重载
  https://pintia.cn/problem-sets/1913050494754209792/exam/problems/type/7?problemSetProblemId=1913050494775181316
*/
// @pintia code=start
#include <iostream>
#include <iomanip>
using namespace std;

class Complex
{
    double real, imag;

public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}

    // Complex + Complex
    Complex operator+(const Complex &rhs) const
    {
        return Complex(real + rhs.real, imag + rhs.imag);
    }
    // Complex - Complex
    Complex operator-(const Complex &rhs) const
    {
        return Complex(real - rhs.real, imag - rhs.imag);
    }
    // Complex * Complex
    Complex operator*(const Complex &rhs) const
    {
        return Complex(real * rhs.real - imag * rhs.imag, real * rhs.imag + imag * rhs.real);
    }
    // Complex / Complex
    Complex operator/(const Complex &rhs) const
    {
        double denom = rhs.real * rhs.real + rhs.imag * rhs.imag;
        return Complex((real * rhs.real + imag * rhs.imag) / denom,
                       (imag * rhs.real - real * rhs.imag) / denom);
    }

    // Complex + int
    Complex operator+(int num) const
    {
        return Complex(real + num, imag);
    }
    // Complex - int
    Complex operator-(int num) const
    {
        return Complex(real - num, imag);
    }
    // Complex * int
    Complex operator*(int num) const
    {
        return Complex(real * num, imag);
    }
    // Complex / int
    Complex operator/(int num) const
    {
        return Complex(real / num, imag);
    }

    // friend int + Complex
    friend Complex operator+(int num, const Complex &c)
    {
        return Complex(num + c.real, c.imag);
    }
    // friend int - Complex
    friend Complex operator-(int num, const Complex &c)
    {
        return Complex(num - c.real, c.imag);
    }
    // friend int * Complex
    friend Complex operator*(int num, const Complex &c)
    {
        return Complex(num * c.real, c.imag);
    }
    // friend int / Complex
    friend Complex operator/(int num, const Complex &c)
    {
        return Complex(num / c.real, c.imag);
    }

    friend ostream &operator<<(ostream &os, const Complex &c)
    {
        os << fixed << setprecision(2) << "(" << c.real << "," << c.imag << "i)";
        return os;
    }
};

int main()
{
    double r1, i1, r2, i2;
    int num;
    cin >> r1 >> i1 >> r2 >> i2 >> num;
    Complex c1(r1, i1), c2(r2, i2);

    cout << "c1+c2=" << c1 + c2 << endl;
    cout << "c1-c2=" << c1 - c2 << endl;
    cout << "c1*c2=" << c1 * c2 << endl;
    cout << "c1/c2=" << c1 / c2 << endl;
    cout << "c1+num=" << c1 + num << endl;
    cout << "c1-num=" << c1 - num << endl;
    cout << "c1*num=" << c1 * num << endl;
    cout << "c1/num=" << c1 / num << endl;
    cout << "num+c1=" << num + c1 << endl;
    cout << "num-c1=" << num - c1 << endl;
    cout << "num*c1=" << num * c1 << endl;
    cout << "num/c1=" << num / c1 << endl;

    return 0;
}
// @pintia code=end