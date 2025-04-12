#include <iostream>
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
                cout << data[i][j];
                if (j != n - 1)
                {
                    cout << " ";
                }
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