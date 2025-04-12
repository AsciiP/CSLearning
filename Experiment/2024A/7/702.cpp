#include <iostream>
#include <string>

using namespace std;

int main()
{
    const string months[] = {
        "January", "February", "March", "April",
        "May", "June", "July", "August",
        "September", "October", "November", "December"
        };

    int monthNumber;
    cin >> monthNumber;

    if (monthNumber < 1 || monthNumber > 12)
    {
        cout << "Invalid!" << endl;
    }
    else
    {
        cout << *(months + monthNumber - 1) << endl;
    }

    return 0;
}