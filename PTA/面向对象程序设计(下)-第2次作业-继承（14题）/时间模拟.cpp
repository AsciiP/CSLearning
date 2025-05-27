/*
  @pintia psid=1910112809178632192 pid=1910112809203798027 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 时间模拟
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798027
*/
// @pintia code=start
#include <iostream>
#include <iomanip>
#include <string>
using namespace std;

class Time
{
protected:
    int hour, minute, second;

public:
    Time(int h = 0, int m = 0, int s = 0) : hour(h), minute(m), second(s) {}
    virtual void operator++()
    {
        second++;
        if (second == 60)
        {
            second = 0;
            minute++;
            if (minute == 60)
            {
                minute = 0;
                hour++;
                if (hour == 24)
                    hour = 0;
            }
        }
    }
    virtual void operator--()
    {
        second--;
        if (second < 0)
        {
            second = 59;
            minute--;
            if (minute < 0)
            {
                minute = 59;
                hour--;
                if (hour < 0)
                    hour = 23;
            }
        }
    }
    virtual void print() const
    {
        cout << setw(2) << setfill('0') << hour << ":"
             << setw(2) << setfill('0') << minute << ":"
             << setw(2) << setfill('0') << second << endl;
    }
    virtual ~Time() {}
};

class Time_12hours : public Time
{
public:
    string type;
    string interval; // AM or PM
    Time_12hours(int h, int m, int s, string ampm) : Time(h, m, s)
    {
        type = "12-hours-time";
        interval = ampm;
    }
    void operator++() override
    {
        second++;
        if (second == 60)
        {
            second = 0;
            minute++;
            if (minute == 60)
            {
                minute = 0;
                hour++;
                if (hour == 12)
                {
                    hour = 0;
                    interval = (interval == "AM") ? "PM" : "AM";
                }
            }
        }
    }
    void operator--() override
    {
        second--;
        if (second < 0)
        {
            second = 59;
            minute--;
            if (minute < 0)
            {
                minute = 59;
                hour--;
                if (hour < 0)
                {
                    hour = 11;
                    interval = (interval == "AM") ? "PM" : "AM";
                }
            }
        }
    }
    void print() const override
    {
        cout << interval << " "
             << setw(2) << setfill('0') << hour << ":"
             << setw(2) << setfill('0') << minute << ":"
             << setw(2) << setfill('0') << second << endl;
    }
};

class Time_24hours : public Time
{
public:
    string type;
    Time_24hours(int h, int m, int s) : Time(h, m, s)
    {
        type = "24-hours-time";
    }
    void operator++() override
    {
        second++;
        if (second == 60)
        {
            second = 0;
            minute++;
            if (minute == 60)
            {
                minute = 0;
                hour++;
                if (hour == 24)
                    hour = 0;
            }
        }
    }
    void operator--() override
    {
        second--;
        if (second < 0)
        {
            second = 59;
            minute--;
            if (minute < 0)
            {
                minute = 59;
                hour--;
                if (hour < 0)
                    hour = 23;
            }
        }
    }
    void print() const override
    {
        cout << setw(2) << setfill('0') << hour << ":"
             << setw(2) << setfill('0') << minute << ":"
             << setw(2) << setfill('0') << second << endl;
    }
};

int main()
{
    int mode, h, m, s, cnt;
    char op;
    while (cin >> mode)
    {
        if (mode == 0)
            break;
        cin >> h >> m >> s >> op >> cnt;
        Time *t = nullptr;
        if (mode == 121)
        {
            t = new Time_12hours(h, m, s, "AM");
        }
        else if (mode == 122)
        {
            t = new Time_12hours(h, m, s, "PM");
        }
        else if (mode == 24)
        {
            t = new Time_24hours(h, m, s);
        }
        if (t)
        {
            for (int i = 0; i < cnt; ++i)
            {
                if (op == '+')
                    ++(*t);
                else
                    --(*t);
            }
            t->print();
            delete t;
        }
    }
    return 0;
}
// @pintia code=end