/*
  @pintia psid=1910112809178632192 pid=1910112809203798017 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 多继承中的构造函数与析构函数
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798017
*/
// @pintia code=start
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

class Dot
{
public:
    float x, y;

    Dot(float x = 0, float y = 0) : x(x), y(y)
    {
        cout << "Dot constructor called" << endl;
    }

    ~Dot()
    {
        cout << "Dot destructor called" << endl;
    }
};

class Color
{
public:
    string color;

    Color(string color = "white") : color(color)
    {
        cout << "Color constructor called" << endl;
    }

    ~Color()
    {
        cout << "Color destructor called" << endl;
    }
};

class ColorCircle : public Dot, public Color
{
private:
    float radius;

public:
    ColorCircle(float x = 0, float y = 0, string color = "white", float radius = 1) : Dot(x, y), Color(color), radius(radius)
    {
        cout << "ColorCircle constructor called" << endl;
    }

    ~ColorCircle()
    {
        cout << "ColorCircle destructor called" << endl;
    }

    float getArea()
    {
        return 3.14 * radius * radius;
    }

    string getColor()
    {
        return color;
    }
    void showInfo()
    {
        cout << "Position: (" << x << ", " << y << "), Radius: " << radius << ", Color: " << color << endl;
    }
};

int main()
{
    float x, y, r;
    string color;
    cin >> x >> y >> r >> color;

    ColorCircle cc(x, y, color, r);
    cout << fixed << setprecision(2);
    cout << "Area: " << cc.getArea() << endl;
    cout << "Color: " << cc.getColor() << endl;
    cc.showInfo();

    return 0;
}
// @pintia code=end