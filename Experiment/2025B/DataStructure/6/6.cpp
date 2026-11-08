/*
  @pintia psid=2051923983973396480 pid=2051923983998562304 compiler=GXX
  ProblemSet: 实验六-图的应用之拓扑排序
  Title: 拓扑排序
  https://pintia.cn/problem-sets/2051923983973396480/exam/problems/type/7?problemSetProblemId=2051923983998562304
*/
// @pintia code=start
#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

int main() {
    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<string> names(n);
    for (int i = 0; i < n; i++) {
        cin >> names[i];
    }

    vector<vector<int>> adj(n);
    vector<int> inDegree(n, 0);

    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        inDegree[v]++;
    }

    // Kahn's Algorithm
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
        }
    }

    vector<int> topoOrder;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        topoOrder.push_back(u);

        for (int v : adj[u]) {
            inDegree[v]--;
            if (inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    bool hasTopoOrder = (topoOrder.size() == (size_t)n);

    cout << "该图拓扑序存在性为 " << (hasTopoOrder ? 1 : 0) << endl;

    if (hasTopoOrder) {
        for (int idx : topoOrder) {
            cout << names[idx] << " ";
        }
        cout << endl;
    } else {
        // 拓扑序不存在时，输出任意内容均可
        cout << "这里输出什么都不重要" << endl;
    }

    return 0;
}
// @pintia code=end