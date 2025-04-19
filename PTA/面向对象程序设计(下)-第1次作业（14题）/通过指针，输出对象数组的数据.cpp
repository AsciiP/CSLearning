/*
  @pintia psid=1903955893807046656 pid=1903955893832212486 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 通过指针，输出对象数组的数据
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212486
*/
// @pintia code=start
#include <iostream>

class Student
{
private:
    int id;
    int score;

public:
    Student(int id = 0, int score = 0) : id(id), score(score) {};
    void print() const
    {
        std::cout << id << " " << score << std::endl;
    }
};

int main()
{
    Student students[5];
    for (int i = 0; i < 5; ++i)
    {
        int id, score;
        std::cin >> id >> score;
        students[i] = Student(id, score);
    }
    for (int i = 0; i < 5; ++i)
    {
        students[i].print();
    }
    return 0;
}
// @pintia code=end