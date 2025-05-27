/*
  @pintia psid=1913050494754209792 pid=1913050494775181314 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part1)-运算符重载
  Title: 日期类的运算符重载
  https://pintia.cn/problem-sets/1913050494754209792/exam/problems/type/7?problemSetProblemId=1913050494775181314
*/
// @pintia code=start
#include <iostream>
using namespace std;

class Date
{
private:
    int year, month, day;
    static int daysInMonth[13];

public:
    Date(int y, int m, int d) : year(y), month(m), day(d) {}
    ~Date() {}

    void print() const
    {
        cout << year << "-" << month << "-" << day << endl;
    }

    Date &operator--()
    {
        if (day > 1)
        {
            --day;
        }
        else
        {
            if (month > 1)
            {
                --month;
            }
            else
            {
                --year;
                month = 12;
            }
            day = daysInMonth[month];
        }
        return *this;
    }
};

int Date::daysInMonth[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

int main()
{
    int y, m, d;
    cin >> y >> m >> d;
    Date date(y, m, d);
    cout << "当前日期为：" << endl;
    date.print();
    --date;
    cout << "前一天日期为：" << endl;
    date.print();
    return 0;
}
// @pintia code=end