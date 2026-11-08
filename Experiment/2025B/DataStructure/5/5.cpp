/*
  @pintia psid=2051923701189206016 pid=2051923701201788928 compiler=GXX
  ProblemSet: 实验五-二叉树应用之哈夫曼树
  Title: 哈夫曼树
  https://pintia.cn/problem-sets/2051923701189206016/exam/problems/type/7?problemSetProblemId=2051923701201788928
*/
// @pintia code=start
#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int n;
    if (!(cin >> n)) return 0;

    // 最小堆
    priority_queue<long long, vector<long long>, greater<long long>> minHeap;

    for (int i = 0; i < n; i++) {
        int weight;
        cin >> weight;
        minHeap.push(weight);
    }

    long long wpl = 0; 

    while (minHeap.size() > 1) {
        long long first = minHeap.top();
        minHeap.pop();
        long long second = minHeap.top();
        minHeap.pop();

        long long newWeight = first + second;
        
        wpl += newWeight;

        minHeap.push(newWeight);
    }

    cout << wpl << endl;

    return 0;
}
// @pintia code=end