/*
  @pintia psid=1903955893807046656 pid=1903955893832212485 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 平均成绩
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212485
*/
// @pintia code=start
#include <iostream>
#include <iomanip>
#include <stack>
#include <string>

class Student
{
private:
    std::string name;
    int score1, score2, score3;
    double averageScore;

public:
    Student(const std::string &name, int score1, int score2, int score3)
        : name(name), score1(score1), score2(score2), score3(score3)
    {
        averageScore = (score1 + score2 + score3) / 3.0;
    }

    void display() const
    {
        std::cout << std::fixed << std::setprecision(2) << name << " " << score1 << " " << score2 << " " << score3 << " " << averageScore << std::endl;
    }

    bool isPassed() const
    {
        return averageScore >= 60;
    }
};

int main()
{
    int n;
    std::cin >> n;
    std::stack<Student> students;
    for (int i = 0; i < n; ++i)
    {
        std::string name;
        int score1, score2, score3;
        std::cin >> name >> score1 >> score2 >> score3;
        students.push(Student(name, score1, score2, score3));
    }
    while (!students.empty())
    {
        Student student = students.top();
        students.pop();
        if (student.isPassed())
        {
            student.display();
        }
    }
    return 0;
}
// @pintia code=end