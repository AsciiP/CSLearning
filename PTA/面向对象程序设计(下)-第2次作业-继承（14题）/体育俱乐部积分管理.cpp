/*
  @pintia psid=1910112809178632192 pid=1910112809203798028 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 体育俱乐部积分管理
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798028
*/
// @pintia code=start
#include <iostream>
#include <string>
using namespace std;

class Ball
{
protected:
    string opponent;

public:
    Ball(const string &opp) : opponent(opp) {}
    virtual void display() = 0;
    virtual ~Ball() {}
};

class Basketball : public Ball
{
    int score1, score2;

public:
    Basketball(const string &opp, int s1, int s2) : Ball(opp), score1(s1), score2(s2) {}
    void display() override
    {
        cout << opponent << " " << (score1 > score2 ? 2 : 1) << endl;
    }
};

class Football : public Ball
{
    int score1, score2;

public:
    Football(const string &opp, int s1, int s2) : Ball(opp), score1(s1), score2(s2) {}
    void display() override
    {
        int pts = 0;
        if (score1 > score2)
            pts = 3;
        else if (score1 == score2)
            pts = 1;
        else
            pts = 0;
        cout << opponent << " " << pts << endl;
    }
};

class Volleyball : public Ball
{
    int score1, score2;

public:
    Volleyball(const string &opp, int s1, int s2) : Ball(opp), score1(s1), score2(s2) {}
    void display() override
    {
        int pts = 0;
        if (score1 == 3 && (score2 == 0 || score2 == 1))
            pts = 3;
        else if (score1 == 3 && score2 == 2)
            pts = 2;
        else if (score1 == 2 && score2 == 3)
            pts = 1;
        else
            pts = 0;
        cout << opponent << " " << pts << endl;
    }
};

int main()
{
    Ball *pb[20];
    int cnt = 0;
    int type;
    string opp, score;
    while (cin >> type && type != 0)
    {
        cin >> opp >> score;
        int s1, s2;
        size_t pos = score.find(':');
        s1 = stoi(score.substr(0, pos));
        s2 = stoi(score.substr(pos + 1));
        if (type == 1)
            pb[cnt++] = new Basketball(opp, s1, s2);
        else if (type == 2)
            pb[cnt++] = new Football(opp, s1, s2);
        else if (type == 3)
            pb[cnt++] = new Volleyball(opp, s1, s2);
    }
    for (int i = 0; i < cnt; ++i)
    {
        pb[i]->display();
        delete pb[i];
    }
    return 0;
}
// @pintia code=end