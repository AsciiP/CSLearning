#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Activity {
    int id, b, e;
};

vector<int> activitySelection(vector<Activity>& acts) {
    sort(acts.begin(), acts.end(), [](const Activity& a, const Activity& b) {
        return a.e < b.e;
    });
    vector<int> selected;
    selected.push_back(acts[0].id);
    int last_end = acts[0].e;
    for (int i = 1; i < (int)acts.size(); ++i) {
        if (acts[i].b >= last_end) {
            selected.push_back(acts[i].id);
            last_end = acts[i].e;
        }
    }
    return selected;
}

int main() {
    // 测试用例：11个活动
    vector<Activity> acts = {
        {1,  1,  4},
        {2,  3,  5},
        {3,  0,  6},
        {4,  5,  7},
        {5,  3,  9},
        {6,  5,  9},
        {7,  6, 10},
        {8,  8, 11},
        {9,  8, 12},
        {10, 2, 14},
        {11, 12, 16}
    };

    vector<int> selected = activitySelection(acts);

    cout << "最多可安排 " << selected.size() << " 个活动：" << endl;
    for (int id : selected) cout << id << " ";
    cout << endl;

    return 0;
}