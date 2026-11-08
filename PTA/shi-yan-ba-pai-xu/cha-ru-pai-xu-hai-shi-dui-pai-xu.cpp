/*
  @pintia psid=2051924408780730368 pid=2051924408793313280 compiler=GXX
  ProblemSet: 实验八-排序
  Title: 插入排序还是堆排序
  https://pintia.cn/problem-sets/2051924408780730368/exam/problems/type/7?problemSetProblemId=2051924408793313280
*/
// @pintia code=start
#include <iostream>
#include <vector>

using namespace std;

void print_vector(const vector<int>& v) {
    for (size_t i = 0; i < v.size() - 1; ++i) {
        cout << v[i] << " ";
    }
    cout << v.back();
}

void down_adjust(vector<int>& heap, int low, int high) {
    int i = low, j = 2 * i + 1;
    while (j <= high) {
        if (j + 1 <= high && heap[j + 1] > heap[j]) {
            j = j + 1;
        }
        if (heap[i] < heap[j]) {
            swap(heap[i], heap[j]);
            i = j;
            j = 2 * i + 1;
        } else {
            break;
        }
    }
}


int main() {
    int n;
    cin >> n;
    vector<int> initial(n), intermediate(n);
    for (int i = 0; i < n; ++i) cin >> initial[i];
    for (int i = 0; i < n; ++i) cin >> intermediate[i];

    vector<int> temp = initial;
    bool is_insertion = false;
    for (int i = 1; i < n; ++i) {
        int current = temp[i];
        int j = i - 1;
        while (j >= 0 && temp[j] > current) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = current;
        // 精确匹配
        if (temp == intermediate) {
            is_insertion = true;
            // Perform one more iteration
            i++;
            if (i < n) {
                current = temp[i];
                j = i - 1;
                while (j >= 0 && temp[j] > current) {
                    temp[j + 1] = temp[j];
                    j--;
                }
                temp[j + 1] = current;
            }
            break;
        }
    }

    if (is_insertion) {
        cout << "Insertion Sort" << endl;
        print_vector(temp);
    } else {
        cout << "Heap Sort" << endl;
        temp = initial;
        // Build heap
        for (int i = n / 2 - 1; i >= 0; --i) {
            down_adjust(temp, i, n - 1);
        }

        for (int i = n - 1; i > 0; --i) {
            swap(temp[0], temp[i]);
            down_adjust(temp, 0, i - 1);
            if (temp == intermediate) {
                // Perform one more iteration
                i--;
                if (i > 0) {
                    swap(temp[0], temp[i]);
                    down_adjust(temp, 0, i - 1);
                }
                break;
            }
        }
        print_vector(temp);
    }

    return 0;
}
// @pintia code=end