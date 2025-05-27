/*
  @pintia psid=1920807589209583616 pid=1920807589230555136 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part1)-虚函数
  Title: 12-1 计算圆形面积
  https://pintia.cn/problem-sets/1920807589209583616/exam/problems/type/6?problemSetProblemId=1920807589230555136
*/
// @pintia code=start
#include <iostream>
using namespace std;

class Shape {
public:
    virtual double getArea() {
        return 0.0;
    }
    virtual void input() {}
    virtual ~Shape() {}
};

class Rectangle : public Shape {
private:
    double length;
    double width;
public:
    Rectangle() : length(0), width(0) {}
    void input() override {
        cin >> length >> width;
    }
    double getArea() override {
        return length * width;
    }
};
// @pintia code=end