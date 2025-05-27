/*
  @pintia psid=1913050494754209792 pid=1913050494775181318 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part1)-运算符重载
  Title: 矩阵的乘法运算
  https://pintia.cn/problem-sets/1913050494754209792/exam/problems/type/7?problemSetProblemId=1913050494775181318
*/
// @pintia code=start
#include <iostream>
#include <iomanip>
using namespace std;

class matrix
{
private:
    int row;
    int column;
    int **mat;

public:
    matrix(int r = 1, int c = 1) : row(r), column(c)
    {
        mat = new int *[row];
        for (int i = 0; i < row; ++i)
        {
            mat[i] = new int[column];
            for (int j = 0; j < column; ++j)
            {
                mat[i][j] = 0;
            }
        }
    }
    matrix(const matrix &other) : row(other.row), column(other.column)
    {
        mat = new int *[row];
        for (int i = 0; i < row; ++i)
        {
            mat[i] = new int[column];
            for (int j = 0; j < column; ++j)
            {
                mat[i][j] = other.mat[i][j];
            }
        }
    }
    ~matrix()
    {
        for (int i = 0; i < row; ++i)
        {
            delete[] mat[i];
        }
        delete[] mat;
    }
    matrix &operator=(const matrix &other)
    {
        if (this == &other)
            return *this;
        for (int i = 0; i < row; ++i)
            delete[] mat[i];
        delete[] mat;
        row = other.row;
        column = other.column;
        mat = new int *[row];
        for (int i = 0; i < row; ++i)
        {
            mat[i] = new int[column];
            for (int j = 0; j < column; ++j)
            {
                mat[i][j] = other.mat[i][j];
            }
        }
        return *this;
    }
    void input()
    {
        for (int i = 0; i < row; ++i)
            for (int j = 0; j < column; ++j)
                cin >> mat[i][j];
    }
    void display()
    {
        // 退化为一个数
        if (row == 1 && column == 1)
        {
            cout << setw(10) << mat[0][0] << endl;
            return;
        }
        // 行向量
        if (row == 1)
        {
            for (int j = 0; j < column; ++j)
            {
                cout << setw(10) << mat[0][j];
            }
            cout << endl;
            return;
        }
        // 列向量
        if (column == 1)
        {
            for (int i = 0; i < row; ++i)
            {
                cout << setw(10) << mat[i][0] << endl;
            }
            return;
        }
        // 普通矩阵
        for (int i = 0; i < row; ++i)
        {
            for (int j = 0; j < column; ++j)
            {
                cout << setw(10) << mat[i][j];
            }
            cout << endl;
        }
    }
    friend matrix operator*(const matrix &a, const matrix &b)
    {
        // 处理退化为数的情况
        if (a.row == 1 && a.column == 1 && b.row == 1 && b.column == 1)
        {
            matrix res(1, 1);
            res.mat[0][0] = a.mat[0][0] * b.mat[0][0];
            return res;
        }
        // 行向量和列向量相乘，得到一个数
        if (a.row == 1 && a.column == b.row && b.column == 1)
        {
            matrix res(1, 1);
            res.mat[0][0] = 0;
            for (int k = 0; k < a.column; ++k)
                res.mat[0][0] += a.mat[0][k] * b.mat[k][0];
            return res;
        }
        // 数与矩阵相乘
        if (a.row == 1 && a.column == 1)
        {
            matrix res(b.row, b.column);
            for (int i = 0; i < b.row; ++i)
                for (int j = 0; j < b.column; ++j)
                    res.mat[i][j] = a.mat[0][0] * b.mat[i][j];
            return res;
        }
        if (b.row == 1 && b.column == 1)
        {
            matrix res(a.row, a.column);
            for (int i = 0; i < a.row; ++i)
                for (int j = 0; j < a.column; ++j)
                    res.mat[i][j] = a.mat[i][j] * b.mat[0][0];
            return res;
        }
        // 普通矩阵乘法
        if (a.column != b.row)
            return matrix(0, 0);
        matrix res(a.row, b.column);
        for (int i = 0; i < a.row; ++i)
        {
            for (int j = 0; j < b.column; ++j)
            {
                res.mat[i][j] = 0;
                for (int k = 0; k < a.column; ++k)
                {
                    res.mat[i][j] += a.mat[i][k] * b.mat[k][j];
                }
            }
        }
        return res;
    }
    int getRow() const { return row; }
    int getColumn() const { return column; }
};

int main()
{
    int r1, c1, r2, c2;
    cin >> r1 >> c1;
    matrix m1(r1, c1);
    m1.input();
    cin >> r2 >> c2;
    matrix m2(r2, c2);
    m2.input();
    if (c1 != r2 && !(r1 == 1 && c1 == 1) && !(r2 == 1 && c2 == 1))
    {
        cout << "Invalid Matrix multiplication!" << endl;
    }
    else
    {
        matrix m3 = m1 * m2;
        m3.display();
    }
    return 0;
}
// @pintia code=end