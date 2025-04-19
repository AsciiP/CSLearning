/*
  @pintia psid=1903955893807046656 pid=1903955893832212490 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 计算全班学生C++课程的总成绩和平均成绩
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212490
*/
// @pintia code=start
#include <iostream>

class Student
{
private:
    int score;
    
public:
    static int totalScore;
    static float averageScore;
    static int studentCount;
    Student(int s = 0) : score(s)
    {
        totalScore += s;
        averageScore = totalScore / (++studentCount);
    }
};

int Student::totalScore = 0;
float Student::averageScore = 0;
int Student::studentCount = 0;

int main()
{
    for (int i = 0; i < 5; ++i)
    {
        int score;
        std::cin >> score;
        Student student(score);
    }
    std::cout << Student::totalScore << std::endl;
    std::cout << Student::averageScore << std::endl;
    return 0;
}
// @pintia code=end