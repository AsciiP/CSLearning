/*
  @pintia psid=1910112809178632192 pid=1910112809203798024 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 计算两点间的距离
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798024
*/
// @pintia code=start
#include <iostream>
#include <cmath>

using namespace std;

class Point_1D
{
protected:
    float x;

public:
    Point_1D(float p = 0) : x(p) {}
    float distance(const Point_1D &p) const
    {
        return fabs(x - p.x);
    }
};

class Point_2D : public Point_1D
{
protected:
    float y;

public:
    Point_2D(float p1 = 0, float p2 = 0) : Point_1D(p1), y(p2) {}
    float distance(const Point_2D &p) const
    {
        return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2));
    }
};

class Point_3D : public Point_2D
{
protected:
    float z;

public:
    Point_3D(float p1 = 0, float p2 = 0, float p3 = 0) : Point_2D(p1, p2), z(p3) {}
    float distance(const Point_3D &p) const
    {
        return sqrt(pow(x - p.x, 2) + pow(y - p.y, 2) + pow(z - p.z, 2));
    }
};

int main()
{
    int type;
    while (cin >> type && type != 0)
    {
        if (type == 1)
        {
            float x;
            cin >> x;
            Point_1D p1(x), origin;
            cout << "Distance from Point " << x << " to Point 0 is " << p1.distance(origin) << endl;
        }
        else if (type == 2)
        {
            float x, y;
            cin >> x >> y;
            Point_2D p1(x, y), origin;
            cout << "Distance from Point(" << x << "," << y << ") to Point(0,0) is " << p1.distance(origin) << endl;
        }
        else if (type == 3)
        {
            float x, y, z;
            cin >> x >> y >> z;
            Point_3D p1(x, y, z), origin;
            cout << "Distance from Point(" << x << "," << y << "," << z << ") to Point(0,0,0) is " << p1.distance(origin) << endl;
        }
    }
    return 0;
}
// @pintia code=end