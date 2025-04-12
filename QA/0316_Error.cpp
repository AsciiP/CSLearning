#include <iostream>
#include <iomanip>
using namespace std;
int main()
{
    double a, b;
    char c;
    cin >> a >> c >> b;
    if (c != '+' && c != '-' && c != '*' && c != '/')
    {
        cout << "Unknown operator!" << endl;
        return 0;
    }
    switch (c)
    {
    case '+':
        a = a + b;
        cout << fixed << setprecision(2) << a << endl;
        break;
    case '-':
        a = a - b;
        cout << fixed << setprecision(2) << a << endl;
        break;
    case '*':
        a = a * b;
        cout << fixed << setprecision(2) << a << endl;
        break;
    case '/':
        if (b != 0)
        {
            a = a / b;
            cout << fixed << setprecision(2) << a << endl;
        }
        else
        {
            cout << "Divisor can not be 0!" << endl;
        }
        break;
    }
}