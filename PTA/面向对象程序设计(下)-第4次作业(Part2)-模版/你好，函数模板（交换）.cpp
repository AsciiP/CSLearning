/*
  @pintia psid=1920808284906205184 pid=1920808284918788097 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part2)-模版
  Title: 你好，函数模板（交换）
  https://pintia.cn/problem-sets/1920808284906205184/exam/problems/type/6?problemSetProblemId=1920808284918788097
*/
// @pintia code=start
template <typename T>
void myswap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}
// @pintia code=end