/*
  @pintia psid=1920807589209583616 pid=1920807589230555138 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part1)-虚函数
  Title: 12-3 图形面积计算
  https://pintia.cn/problem-sets/1920807589209583616/exam/problems/type/6?problemSetProblemId=1920807589230555138
*/
// @pintia code=start
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

class Shape {
public:
    virtual double getArea() const = 0;
    virtual void input() = 0;
    virtual ~Shape() {}
};

class Rectangle : public Shape {
private:
    double length;
    double width;
public:
    Rectangle() : length(0), width(0) {}
    double getArea() const override {
        return length * width;
    }
    void input() override {
        cout << "Enter length:" << endl;
        cin >> length;
        cout << "Enter width:" << endl;
        cin >> width;
    }
};

class Circle : public Shape {
private:
    double radius;
public:
    Circle() : radius(0) {}
    double getArea() const override {
        return M_PI * radius * radius;
    }
    void input() override {
        cout << "Enter radius:" << endl;
        cin >> radius;
    }
};

class Triangle : public Shape {
private:
    double base;
    double height;
public:
    Triangle() : base(0), height(0) {}
    double getArea() const override {
        return 0.5 * base * height;
    }
    void input() override {
        cout << "Enter base:" << endl;
        cin >> base;
        cout << "Enter height:" << endl;
        cin >> height;
    }
};
// @pintia code=end