/*
  @pintia psid=1910112809178632192 pid=1910112809203798018 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 继承中构造函数和析构函数的调用顺序
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798018
*/
// @pintia code=start
#include <iostream>

using namespace std;

class Person
{
protected:
    string name;
    int age;

public:
    Person(string n, int a) : name(n), age(a)
    {
        cout << "构造Person: " << name << endl;
    }
    virtual ~Person()
    {
        cout << "析构Person: " << name << endl;
    }
    virtual void show() const
    {
        cout << "姓名: " << name << ", 年龄: " << age << endl;
    }
};

class Teacher : public Person
{
private:
    string subject;

public:
    Teacher(string n, int a, string s) : Person(n, a), subject(s)
    {
        cout << "构造Teacher: " << name << endl;
    }
    ~Teacher()
    {
        cout << "析构Teacher: " << name << endl;
    }
    void show() const override
    {
        cout << "姓名: " << name << ", 年龄: " << age << ", 科目: " << subject << endl;
    }
};

class Student : public Person
{
private:
    int grade;

public:
    Student(string n, int a, int g) : Person(n, a), grade(g)
    {
        cout << "构造Student: " << name << endl;
    }
    ~Student()
    {
        cout << "析构Student: " << name << endl;
    }
    void show() const override
    {
        cout << "姓名: " << name << ", 年龄: " << age << ", 年级: " << grade << endl;
    }
};

int main()
{
    string name, subject;
    int age, grade;

    cin >> name;
    cin >> age;
    cin >> subject;
    Teacher t(name, age, subject);

    cin >> name;
    cin >> age;
    cin >> grade;
    Student s(name, age, grade);

    t.show();
    s.show();

    return 0;
}
// @pintia code=end