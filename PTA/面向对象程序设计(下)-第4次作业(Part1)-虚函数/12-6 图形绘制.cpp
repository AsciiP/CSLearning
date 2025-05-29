/*
  @pintia psid=1920807589209583616 pid=1920807589230555143 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part1)-虚函数
  Title: 12-6 图形绘制
  https://pintia.cn/problem-sets/1920807589209583616/exam/problems/type/7?problemSetProblemId=1920807589230555143
*/
// @pintia code=start
#include <iostream>
#include <string>
using namespace std;

class Shape
{
public:
    virtual void draw() = 0;
    virtual void inputInfo() = 0;
    virtual ~Shape() {}
};

class Color
{
protected:
    string colorName;

public:
    Color(const string &name) : colorName(name) {}
    string getColor() const { return colorName; }
    virtual ~Color() {}
};

class ColoredShape : public Shape, public Color
{
public:
    ColoredShape(const string &name) : Color(name) {}
    virtual ~ColoredShape() {}
    void inputInfo() override
    {
        // For derived class to call if needed
    }
    void draw() override
    {
        // For derived class to call if needed
    }
};

class Rectangle : public ColoredShape
{
private:
    double width, height;

public:
    Rectangle(const string &color) : ColoredShape(color), width(0), height(0) {}
    void inputInfo() override
    {
        cout << "Enter rectangle width:";
        cin >> width;
        cout << "Enter rectangle height:";
        cin >> height;
    }
    void draw() override
    {
        cout << "Drawing Rectangle with width " << width << ", height " << height
             << " and color " << getColor() << endl;
    }
};

class Circle : public ColoredShape
{
private:
    double radius;

public:
    Circle(const string &color) : ColoredShape(color), radius(0) {}
    void inputInfo() override
    {
        cout << "Enter circle radius:";
        cin >> radius;
    }
    void draw() override
    {
        cout << "Drawing Circle with radius " << radius
             << " and color " << getColor() << endl;
    }
};

class Triangle : public ColoredShape
{
private:
    double a, b, c;

public:
    Triangle(const string &color) : ColoredShape(color), a(0), b(0), c(0) {}
    void inputInfo() override
    {
        cout << "Enter triangle side 1:";
        cin >> a;
        cout << "Enter triangle side 2:";
        cin >> b;
        cout << "Enter triangle side 3:";
        cin >> c;
    }
    void draw() override
    {
        cout << "Drawing Triangle with sides " << a << ", " << b << ", " << c
             << " and color " << getColor() << endl;
    }
};

int main()
{
    Shape *shapes[3];
    shapes[0] = new Rectangle("Red");
    shapes[1] = new Circle("Blue");
    shapes[2] = new Triangle("Green");

    for (int i = 0; i < 3; ++i)
    {
        cout << "Input info for shape " << i + 1 << ":" << endl;
        shapes[i]->inputInfo();
    }

    for (int i = 0; i < 3; ++i)
    {
        cout << "Drawing shape " << i + 1 << ":" << endl;
        shapes[i]->draw();
    }

    for (int i = 0; i < 3; ++i)
    {
        delete shapes[i];
    }

    return 0;
}
// @pintia code=end