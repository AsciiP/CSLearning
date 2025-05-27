/*
  @pintia psid=1913050494754209792 pid=1913050494775181313 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part1)-运算符重载
  Title: 有两个矩阵a和b，均为2行3列。求两个矩阵之和。采用友元函数重载运算符“+”，使之能用于矩阵相加（如c=a+b）。
  https://pintia.cn/problem-sets/1913050494754209792/exam/problems/type/7?problemSetProblemId=1913050494775181313
*/
// @pintia code=start
#include <iostream>
using namespace std;

class Matrix
{
private:
    int data[2][3];

public:
    Matrix() {}
    void input()
    {
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < 3; ++j)
                cin >> data[i][j];
    }
    void output() const
    {
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                cout << data[i][j];
                cout << " ";
            }
            cout << endl;
        }
    }
    friend Matrix operator+(const Matrix &m1, const Matrix &m2);
};

Matrix operator+(const Matrix &m1, const Matrix &m2)
{
    Matrix res;
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            res.data[i][j] = m1.data[i][j] + m2.data[i][j];
    return res;
}

int main()
{
    Matrix a, b, c;
    a.input();
    b.input();
    c = a + b;
    c.output();
    return 0;
}
// @pintia code=end