#include <iostream>
#include <iomanip>
using namespace std;

double piLeibniz(double e)
{
    double pi = 0;
    double i = 1;
    double symbol = 1;
    while (1 / i >= e)
    {
        pi += symbol * 1 / i;
        i += 2;
        symbol *= -1;
    }
    return pi * 4;
}

int main()
{
    double e;
    cin >> e;
    cout << fixed << setprecision(6) << piLeibniz(e) << endl;
}