#include<iostream>
#include<vector>

using namespace std;

int main()
{
	int n;
	cin >> n;

	vector<int> a(n);

	for (int i = 0; i < n; ++i) {
		cin >> a[i];
	}

	int m;
	cin >> m;

	vector<int> temp(n);
	int *aIndex = &a[0];
	int *tempIndex = &temp[0];

    for (int i = 0; i < n; ++i) {
        // *(temp + ((i + m) % n)) = *(a + i);
        if (i + m < n) {
            *(tempIndex + i + m) = *(aIndex + i);
        } else {
            *(tempIndex + i + m - n) = *(aIndex + i);
        }
    }

	for (int i = 0; i < n; ++i) {
		cout << temp[i] << " ";
	}
	cout << endl;
}