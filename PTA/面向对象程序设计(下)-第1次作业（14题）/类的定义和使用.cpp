/*
  @pintia psid=1903955893807046656 pid=1903955893832212480 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 类的定义和使用
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212480
*/
// @pintia code=start
#include <iostream>

class Date
{
public:
    int year;
    int month;
    int day;
public:
    Date(int y = 2000, int m = 1, int d = 1)
        :year(y), month(m), day(d){}
    bool isLeapYear()
    {
        return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
    }
    int getYear()
    {
        return year;
    }
    int getMonth()
    {
        return month;
    }
    int getDay()
    {
        return day;
    }
};

int main()
{
    Date date;
    std::cin >> date.year >> date.month >> date.day;
    int days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dayIndex = 0;
    for (int m = 1; m < date.month; m++)
    {
        dayIndex += days[m];
    }
    dayIndex += date.day;
    if (date.isLeapYear() && date.month > 2)
    {
        dayIndex += 1;
    }
    std::cout << dayIndex;
    return 0;
}
// @pintia code=end