/*
  @pintia psid=1903955893807046656 pid=1903955893832212492 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 计算高考状元
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212492
*/
// @pintia code=start
#include <iostream>

class Student
{
private:
    int score[4];
    int totalScore;
public:
    Student(int s1, int s2, int s3, int s4) : score{s1, s2, s3, s4}
    {
        totalScore = 0;
        for (int i = 0; i < 4; ++i)
        {
            totalScore += score[i];
        }
    }

    int getTotalScore() const
    {
        return totalScore;
    }

    void print() const
    {
        std::cout << totalScore << std::endl;
    }
};

int main()
{
    std::string name;
    int s1, s2, s3, s4, maxScore = 0;

    while (std::cin >> name && name != "0")
    {
        std::cin >> s1 >> s2 >> s3 >> s4;
        Student student(s1, s2, s3, s4);
        if (student.getTotalScore() > maxScore)
        {
            maxScore = student.getTotalScore();
        }
    }

    std::cout << maxScore << std::endl;
}
// @pintia code=end