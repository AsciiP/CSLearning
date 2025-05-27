/*
  @pintia psid=1913050494754209792 pid=1913050494775181315 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part1)-运算符重载
  Title: 自定义类mystring实现运算符=重载
  https://pintia.cn/problem-sets/1913050494754209792/exam/problems/type/7?problemSetProblemId=1913050494775181315
*/
// @pintia code=start
#include <iostream>
#include <cstring>
using namespace std;

class mystring
{
private:
    char *buf;

public:
    mystring()
    {
        buf = nullptr;
        cout << "construct 0" << endl;
    }
    mystring(const char *s)
    {
        buf = new char[strlen(s) + 1];
        strcpy(buf, s);
        cout << "construct 1" << endl;
    }
    mystring(const mystring &other)
    {
        if (other.buf)
        {
            buf = new char[strlen(other.buf) + 1];
            strcpy(buf, other.buf);
        }
        else
        {
            buf = nullptr;
        }
    }
    ~mystring()
    {
        cout << "destruct" << endl;
        delete[] buf;
    }
    void print() const
    {
        if (buf)
            cout << buf << endl;
    }
    mystring &operator=(const mystring &rhs)
    {
        if (this != &rhs)
        {
            delete[] buf;
            const char *prefix = "c++";
            buf = new char[strlen(prefix) + strlen(rhs.buf) + 1];
            strcpy(buf, prefix);
            strcat(buf, rhs.buf);
        }
        return *this;
    }
};

int main()
{
    char ch[55];
    cin >> ch;
    mystring str1(ch);
    str1.print();
    mystring str2;
    str2 = str1;
    str2.print();
    return 0;
}
// @pintia code=end