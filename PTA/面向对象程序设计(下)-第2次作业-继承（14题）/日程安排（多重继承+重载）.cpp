/*
  @pintia psid=1910112809178632192 pid=1910112809203798023 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 日程安排（多重继承+重载）
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798023
*/
// @pintia code=start
#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Date
{
protected:
    int year;
    int month;
    int day;

public:
    Date(int y = 0, int m = 0, int d = 0) : year(y), month(m), day(d) {}
};

class Time
{
protected:
    int hour;
    int minute;
    int second;

public:
    Time(int h = 0, int m = 0, int s = 0) : hour(h), minute(m), second(s) {}
};

class Schedule : public Date, public Time
{
private:
    int ID;

public:
    Schedule(int id, int y, int m, int d, int h, int min, int s)
        : Date(y, m, d), Time(h, min, s), ID(id) {}

    bool operator<(const Schedule &s2) const
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

    void print() const
    {
        cout << "The urgent schedule is No." << ID << ": "
             << year << "/" << month << "/" << day << " "
             << hour << ":" << minute << ":" << second << endl;
    }
};

int main()
{
    vector<Schedule> schedules;
    int id, y, m, d, h, min, s;
    char slash, colon;

    while (cin >> id && id != 0)
    {
        cin >> y >> slash >> m >> slash >> d >> h >> colon >> min >> colon >> s;
        schedules.emplace_back(id, y, m, d, h, min, s);
    }

    if (!schedules.empty())
    {
        Schedule earliest = schedules[0];
        for (const auto &schedule : schedules)
        {
            if (schedule < earliest)
            {
                earliest = schedule;
            }
        }
        earliest.print();
    }

    return 0;
}
// @pintia code=end