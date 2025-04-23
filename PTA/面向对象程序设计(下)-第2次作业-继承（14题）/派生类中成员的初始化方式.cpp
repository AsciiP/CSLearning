/*
  @pintia psid=1910112809178632192 pid=1910112809203798021 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 派生类中成员的初始化方式
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798021
*/
// @pintia code=start
#include <iostream>
#include <string>

using namespace std;

class Shape {
protected:
    string name;  // 图形名称
    int sides;    // 边数
public:
    Shape(string n, int s) : name(n), sides(s) {
        cout << "Shape constructor: " << name << endl;
    }
    virtual ~Shape() {
        cout << "Shape destructor: " << name << endl;
    }
    virtual void display() = 0;  // 纯虚函数
};

class Rectangle : public Shape {
private:
    int width, height;
public:
    Rectangle() : Shape("Default Rectangle", 4), width(0), height(0) {}
    Rectangle(int w, int h) : Shape("Rectangle", 4), width(w), height(h) {}
    void display() override {
        cout << "Rectangle: sides=" << sides << ", width=" << width << ", height=" << height << endl;
    }
};

class RegularPolygon : public Shape {
private:
    int sideLength;
public:
    RegularPolygon() : Shape("Default Polygon", 0), sideLength(0) {}
    RegularPolygon(int s, int l) : Shape("Hexagon", s), sideLength(l) {}
    void display() override {
        cout << "Regular Polygon: sides=" << sides << ", sideLength=" << sideLength << endl;
    }
};

int main() {
    // 测试默认构造
    Rectangle r1;
    RegularPolygon p1;
    
    // 测试参数化构造
    Rectangle r2(5, 10);
    RegularPolygon p2(6, 8);
    
    // 存储指针
    Shape* shapes[] = {&r1, &p1, &r2, &p2};
    
    // 显示所有图形信息
    for (auto shape : shapes) {
        shape->display();
    }
    
    return 0;
}
// @pintia code=end