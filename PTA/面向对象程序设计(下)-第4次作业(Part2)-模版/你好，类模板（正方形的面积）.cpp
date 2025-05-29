/*
  @pintia psid=1920808284906205184 pid=1920808284918788098 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part2)-模版
  Title: 你好，类模板（正方形的面积）
  https://pintia.cn/problem-sets/1920808284906205184/exam/problems/type/6?problemSetProblemId=1920808284918788098
*/
// @pintia code=start
template <typename T>
class Square
{
private:
    T width;

public:
    Square(T w) : width(w) {}
    T getArea()
    {
        return width * width;
    }
};
// @pintia code=end