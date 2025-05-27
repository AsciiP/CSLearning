/*
  @pintia psid=1913050494754209792 pid=1913050494775181317 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part1)-运算符重载
  Title: 复数的输入输出运算符重载
  https://pintia.cn/problem-sets/1913050494754209792/exam/problems/type/7?problemSetProblemId=1913050494775181317
*/
// @pintia code=start
#include <iostream>
using namespace std;

class Complex {
private:
    double real, imag;
public:
    Complex(double r = 0, double i = 0) : real(r), imag(i) {}
    friend istream& operator>>(istream& in, Complex& c) {
        char op, i_char;
        in >> c.real >> op >> c.imag >> i_char;
        if (op == '-') c.imag = -c.imag;
        return in;
    }
    friend ostream& operator<<(ostream& out, const Complex& c) {
        out << c.real;
        if (c.imag >= 0)
            out << "+" << c.imag << "i";
        else
            out << c.imag << "i";
        return out;
    }
};

int main() {
    Complex c;
    cin >> c;
    cout << c << endl;
    return 0;
}
// @pintia code=end