/*
  @pintia psid=2051924236764078080 pid=2051924236776660992 compiler=GXX
  ProblemSet: 实验七-折半查找
  Title: 折半查找
  https://pintia.cn/problem-sets/2051924236764078080/exam/problems/type/7?problemSetProblemId=2051924236776660992
*/
// @pintia code=start
#include <iostream>
#include <vector>

using namespace std;

int binary_search(const vector<int>& arr, int key) {
    int low = 0;
    int high = arr.size() - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid] == key) {
            return mid + 1;
        } else if (arr[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return 0;
}

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int i = 0; i < n; ++i) {
        cin >> arr[i];
    }

    int key;
    while (cin >> key && key != -1) {
        int result = binary_search(arr, key);
        cout << result << " ";
    }
    cout << endl;

    return 0;
}
// @pintia code=end