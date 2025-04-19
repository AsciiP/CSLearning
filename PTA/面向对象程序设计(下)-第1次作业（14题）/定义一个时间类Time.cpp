/*
  @pintia psid=1903955893807046656 pid=1903955893832212482 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 定义一个时间类Time
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212482
*/
// @pintia code=start
#include <iostream>

class Time
{
private:
    int days;
    int hours;
    int minutes;
    void normalize()
    {
        hours += minutes / 60;
        minutes %= 60;
        days += hours / 24;
        hours %= 24;
    }
public:
    Time(int d, int h, int m): days(d), hours(h), minutes(m)
    {
        normalize();
    }
    Time sum(const Time &t)
    {
        return Time(days + t.days, hours + t.hours, minutes + t.minutes);
    }
    void show() const
    {
        std::cout << days << ' ' << hours << ' ' << minutes;
    }
};

int main()
{
    int d1, h1, m1, d2, h2, m2;
    std::cin >> d1 >> h1 >> m1 >> d2 >> h2 >> m2;
    Time time1(d1, h1, m1);
    Time time2(d2, h2, m2);
    Time result = time1.sum(time2);
    result.show();
    return 0;
}
// @pintia code=end