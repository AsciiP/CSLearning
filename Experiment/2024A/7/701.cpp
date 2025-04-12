#include <iostream>


using namespace std;

int main()
{
    int n;
    cin >> n;

    int a[n];

    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    int m;
    cin >> m;

    int temp[n];

    

    for (int i = 0; i < n; ++i) {
        // *(temp + ((i + m) % n)) = *(a + i);
        if (i + m < n) {
            *(temp + i + m) = *(a + i);
        } else {
            *(temp + i + m - n) = *(a + i);
        }
    }

    for (int i = 0; i < n; ++i) {
        cout << *(temp + i) << " ";
    }
    cout << endl;
}