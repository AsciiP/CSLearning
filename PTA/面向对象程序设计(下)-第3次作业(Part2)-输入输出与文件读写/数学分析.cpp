/*
  @pintia psid=1913051309887856640 pid=1913051309908828161 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part2)-输入输出与文件读写
  Title: 数学分析
  https://pintia.cn/problem-sets/1913051309887856640/exam/problems/type/7?problemSetProblemId=1913051309908828161
*/
// @pintia code=start
#include <iostream>
using namespace std;

int main() {
    int x1, y1, x2, y2, x3, y3;
    cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;
    int x4, y4;
    if (x1 == x2) x4 = x3;
    else if (x1 == x3) x4 = x2;
    else x4 = x1;
    if (y1 == y2) y4 = y3;
    else if (y1 == y3) y4 = y2;
    else y4 = y1;
    cout << x4 << " " << y4 << endl;
    return 0;
}
// @pintia code=end