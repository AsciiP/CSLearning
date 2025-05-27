/*
  @pintia psid=1910112809178632192 pid=1910112809203798029 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 制作评分器
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798029
*/
// @pintia code=start
#include <iostream>
#include <iomanip>
using namespace std;

class Grade
{
private:
    char letter;
    float score;
    void calcGrade()
    {
        if (score >= 90)
            letter = 'A';
        else if (score >= 80)
            letter = 'B';
        else if (score >= 70)
            letter = 'C';
        else if (score >= 60)
            letter = 'D';
        else
            letter = 'F';
    }

public:
    void setScore(float s)
    {
        score = s;
        calcGrade();
    }
    float getScore()
    {
        return score;
    }
    char getLetter()
    {
        return letter;
    }
};

class Test : public Grade
{
private:
    int numQuestions;
    float pointsEach;
    int numMissed;

public:
    Test(int questions, int missed)
    {
        numQuestions = questions;
        numMissed = missed;
        pointsEach = 100.0f / numQuestions;
        float s = 100.0f - numMissed * pointsEach;
        if (s < 0)
            s = 0;
        setScore(s);
    }
};

int main()
{
    int total, missed;
    cin >> total >> missed;
    Test t(total, missed);
    cout << "The score is:" << fixed << setprecision(2) << t.getScore() << endl;
    cout << "The grade is:" << t.getLetter() << endl;
    return 0;
}
// @pintia code=end