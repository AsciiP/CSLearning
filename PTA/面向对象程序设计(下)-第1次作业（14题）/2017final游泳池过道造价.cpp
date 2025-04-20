/*
  @pintia psid=1903955893807046656 pid=1903955893832212481 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 2017final游泳池过道造价
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212481
*/
// @pintia code=start
#include <iostream>
#define PI 3.14159

class Cir
{
private:
    double radius;
public:
    Cir(double r):
        radius(r){}
    double area()
    {
        return PI * radius * radius;
    }
};

class Rectangle
{
private:
    double length;
    double width;
public:
    Rectangle(double l, double w)
        :length(l), width(w){}
    double area()
    {
        return length * width;
    }
};

int main()
{
    double r, l, w;
    std::cin >> r >> l >> w;
    Cir pool(r);
    Rectangle land(l, w);
    double price = (land.area() - pool.area()) * 20.0;
    std::cout << price;
    return 0;
}

// @pintia code=end