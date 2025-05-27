/*
  @pintia psid=1910112809178632192 pid=1910112809203798026 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 复数的实部和虚部
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798026
*/
// @pintia code=start
#include <iostream>
#include <string>

const size_t notFound = std::string::npos;

class Complex
{
private:
    std::string expression;
    float real = 0;
    float imag = 0;

public:
    Complex(const std::string &exp)
    {
        expression = exp;
        size_t iPos = exp.rfind('i'); // Try to find i
        if (iPos == notFound)         // No i
        {
            real = std::stof(exp);
            imag = 0;
            return;
        }

        // Have i
        size_t signPos = exp.rfind('+', iPos); // Try to find sign of i
        if (signPos == notFound)
        {
            signPos = exp.rfind('-', iPos);
        }

        std::string realPart, imagPart;

        if (signPos != notFound && signPos != 0) // Sign of i, have real
        {
            realPart = exp.substr(0, signPos);
            imagPart = exp.substr(signPos, iPos - signPos);
        }
        else // No real
        {
            realPart = "0";
            imagPart = exp.substr(0, iPos);
        }

        if (imagPart == "+" || imagPart == "-")
        {
            imagPart += "1"; // "3", "+" -> "3", "+1"
        }

        real = std::stof(realPart);
        imag = std::stof(imagPart);
    }
    void display() const
    {
        std::cout << "complex " << expression << '\n';
        std::cout << "the real part is " << real << '\n';
        std::cout << "and the imaginary part is " << imag << std::endl;
    }
};

int main()
{
    std::string input;
    std::cin >> input;
    Complex c(input);
    c.display();
    return 0;
}
// @pintia code=end