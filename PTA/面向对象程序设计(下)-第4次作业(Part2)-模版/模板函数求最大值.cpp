/*
  @pintia psid=1920808284906205184 pid=1920808284918788096 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part2)-模版
  Title: 模板函数求最大值
  https://pintia.cn/problem-sets/1920808284906205184/exam/problems/type/6?problemSetProblemId=1920808284918788096
*/
// @pintia code=start
template<typename T>
T MAX_VALUE(T a, T b, T c) {
    T max = a;
    if (b > max) max = b;
    if (c > max) max = c;
    return max;
}
// @pintia code=end