/*
  @pintia psid=1903955893807046656 pid=1903955893832212493 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: （类与对象）实现一个通用的三维盒子类
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212493
*/
// @pintia code=start
#include <iostream>

class Box
{
private:
    double length, width, height;

public:
    Box(double l = 1, double w = 1, double h = 1) : length(l), width(w), height(h) {}

    double getVolume() const
    {
        return length * width * height;
    }

    double getArea() const
    {
        return 2 * (length * width + width * height + height * length);
    }
};

int main()
{
    double l, w, h;
    std::cin >> l >> w >> h;
    Box box1(l, w, h);
    std::cout << box1.getVolume() << ' ';
    std::cout << box1.getArea() << std::endl;
    std::cin >> l >> w >> h;
    Box box2(l, w, h);
    std::cout << box2.getVolume() << ' ';
    std::cout << box2.getArea() << std::endl;

    return 0;
}
// @pintia code=end