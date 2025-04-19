/*
  @pintia psid=1903955893807046656 pid=1903955893832212484 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 动态二维数组的转置
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212484
*/
// @pintia code=start
#include <iostream>
#include <iomanip>
using namespace std;

class Matrix
{
private:
    int m, n;
    int **data;

public:
    Matrix(int m, int n) : m(m), n(n)
    {
        data = new int *[m];
        for (int i = 0; i < m; i++)
        {
            data[i] = new int[n];
        }
    }
    void ReadMatrix()
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> data[i][j];
            }
        }
    }
    void WriteMatrix()
    {
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cout << setw(5) << data[i][j];
            }
            cout << endl;
        }
    }
    void Transpose(Matrix &B)
    {
        B.m = n;
        B.n = m;
        for (int i = 0; i < m; i++)
        {
            for (int j = 0; j < n; j++)
            {
                B.data[j][i] = data[i][j];
            }
        }
    }
};

int main()
{
    int m, n;
    cin >> m >> n;
    Matrix A(m, n);
    A.ReadMatrix();
    A.WriteMatrix();
    Matrix B(n, m);
    A.Transpose(B);
    B.WriteMatrix();
    return 0;
}

// @pintia code=end