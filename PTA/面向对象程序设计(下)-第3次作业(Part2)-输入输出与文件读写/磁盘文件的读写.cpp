/*
    @pintia psid=1913051309887856640 pid=1913051309908828160 compiler=GXX
    ProblemSet: 面向对象程序设计(下)-第3次作业(Part2)-输入输出与文件读写
    Title: 磁盘文件的读写
    https://pintia.cn/problem-sets/1913051309887856640/exam/problems/type/7?problemSetProblemId=1913051309908828160
*/
// @pintia code=start
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
using namespace std;

class Student
{
public:
    int num;
    string name;
    int age;
    double maths, english, chinese, ave;
    Student(int n, string nm, int a, double m, double e, double c)
        : num(n), name(nm), age(a), maths(m), english(e), chinese(c)
    {
        ave = (maths + english + chinese) / 3.0;
    }
};

int main()
{
    Student stu[5] = {
        Student(1, "A", 19, 80, 79, 67),
        Student(2, "B", 20, 90, 68, 43),
        Student(3, "C", 19, 56, 48, 29),
        Student(4, "D", 20, 93, 44, 57),
        Student(5, "E", 19, 33, 55, 74)
    };

    ofstream fout("STUD.DAT");
    for (int i = 0; i < 5; ++i)
    {
        fout << stu[i].num << " " << stu[i].name << " " << fixed << setprecision(4) << stu[i].ave << endl;
    }
    fout.close();

    ifstream fin("STUD.DAT");
    int num;
    string name;
    double ave;
    while (fin >> num >> name >> ave)
    {
        cout << num << " " << name << " ";
        cout << fixed << setprecision(4);
        if (ave == (int)ave)
            cout << (int)ave << endl;
        else
            cout << ave << endl;
    }
    fin.close();
    return 0;
}
// @pintia code=end