/*
  @pintia psid=1903955893807046656 pid=1903955893832212483 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: LX704 三维坐标
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212483
*/
// @pintia code=start
#include <math.h>
#include <iostream>
using namespace std;

// BEGIN
class TPoint
{
public:
    double x, y, z;

public:
    TPoint(double x_ = 10, double y_ = 10, double z_ = 10)
        : x(x_), y(y_), z(z_) {}
    void Show()
    {
        cout << '(' << x << ',' << y << ',' << z << ')';
    }
};

double len(TPoint &a, TPoint &b)
{
    return sqrt((a.x - b.x) * (a.x - b.x) +
                (a.y - b.y) * (a.y - b.y) +
                (a.z - b.z) * (a.z - b.z));
}

void move(TPoint &a, TPoint &b)
{
    a.x = b.x;
    a.y = b.y;
    a.z = b.z;
}
// END

int main()
{
    int x, y, z;
    TPoint a(1, 2, 3);
    TPoint b;
    TPoint c(10);
    cin >> x >> y >> z;
    TPoint d(x, y, z);
    cout << len(a, d) << endl;
    move(a, d);
    a.Show();
    return 0;
}
// @pintia code=end