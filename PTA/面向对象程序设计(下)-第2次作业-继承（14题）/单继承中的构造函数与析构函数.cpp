/*
  @pintia psid=1910112809178632192 pid=1910112809203798016 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 单继承中的构造函数与析构函数
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798016
*/
// @pintia code=start
#include <iostream>

using namespace std;

const double PI = 3.14;

class Dot
{
private:
    double x, y;

public:
    Dot(double x = 0, double y = 0) : x(x), y(y)
    {
        cout << "Dot constructor called" << endl;
    }
    ~Dot()
    {
        cout << "Dot destructor called" << endl;
    }
};

class Cir : public Dot
{
private:
    double radius;

public:
    Cir(double x = 0, double y = 0, double r = 0) : Dot(x, y), radius(r)
    {
        cout << "Cir constructor called" << endl;
    }
    ~Cir()
    {
        cout << "Cir destructor called" << endl;
    }
    double getArea() const
    {
        return PI * radius * radius;
    }
};

int main()
{
    double x, y, r;
    cin >> x >> y >> r;
    Cir c(x, y, r);
    cout << c.getArea() << endl;

    return 0;
}
// @pintia code=end