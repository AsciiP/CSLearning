/*
  @pintia psid=1913050494754209792 pid=1913050494779375616 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part1)-运算符重载
  Title: 日程安排（多重继承+重载）
  https://pintia.cn/problem-sets/1913050494754209792/exam/problems/type/7?problemSetProblemId=1913050494779375616
*/
// @pintia code=start
#include <iostream>
using namespace std;

class Date
{
protected:
    int year, month, day;

public:
    Date(int y = 0, int m = 0, int d = 0) : year(y), month(m), day(d) {}
};

class Time
{
protected:
    int hour, minute, second;

public:
    Time(int h = 0, int m = 0, int s = 0) : hour(h), minute(m), second(s) {}
};

class Schedule : public Date, public Time
{
public:
    int ID;
    Schedule(int id = 0, int y = 0, int m = 0, int d = 0, int h = 0, int mi = 0, int s = 0)
        : Date(y, m, d), Time(h, mi, s), ID(id) {}

    bool operator<(const Schedule &s2)
    {
        if (year != s2.year)
            return year < s2.year;
        if (month != s2.month)
            return month < s2.month;
        if (day != s2.day)
            return day < s2.day;
        if (hour != s2.hour)
            return hour < s2.hour;
        if (minute != s2.minute)
            return minute < s2.minute;
        return second < s2.second;
    }

    void print()
    {
        cout << "The urgent schedule is No." << ID << ": "
             << year << "/" << month << "/" << day << " "
             << hour << ":" << minute << ":" << second << endl;
    }
};

int main()
{
    int id, y, m, d, h, mi, s;
    Schedule urgent;
    bool first = true;
    while (cin >> id && id != 0)
    {
        char ch1, ch2, ch3;
        cin >> y >> ch1 >> m >> ch2 >> d >> h >> ch3 >> mi >> ch3 >> s;
        Schedule tmp(id, y, m, d, h, mi, s);
        if (first || tmp < urgent)
        {
            urgent = tmp;
            first = false;
        }
    }
    if (!first)
        urgent.print();
    return 0;
}
// @pintia code=end