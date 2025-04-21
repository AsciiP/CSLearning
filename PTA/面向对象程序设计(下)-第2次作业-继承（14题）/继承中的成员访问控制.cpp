/*
  @pintia psid=1910112809178632192 pid=1910112809203798019 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 继承中的成员访问控制
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798019
*/
// @pintia code=start
#include <iostream>

using namespace std;

class Person
{
private:
    string idNumber;

protected:
    string name;

public:
    int age;
    Person(string id, string n, int a) : idNumber(id), name(n), age(a) {}
    void displayBasicInfo()
    {
        cout << "姓名：" << name << "，年龄：" << age << endl;
    }
};

class Teacher : public Person
{
private:
    string employeeId;

protected:
    string department;

public:
    Teacher(string id, string n, int a, string empId, string dept) : Person(id, n, a), employeeId(empId), department(dept) {}
    void displayTeacherInfo()
    {
        cout << "姓名：" << name << "，年龄：" << age << endl;
        cout << "教职工号：" << employeeId << "，院系：" << department << endl;
    }
};

class Student : protected Person
{
private:
    string studentId;

protected:
    string getStudentId() const
    {
        return studentId;
    }

public:
    Student(string id, string n, int a, string stuId) : Person(id, n, a), studentId(stuId) {}

    void displayStudentInfo()
    {
        cout << "姓名：" << name << "，年龄：" << age << endl;
        cout << "学号：" << studentId << endl;
    }
};

class GraduateStudent : private Student
{
private:
    string researchTopic;

public:
    GraduateStudent(string id, string n, int a, string stuId, string topic) : Student(id, n, a, stuId), researchTopic(topic) {}
    void displayGraduateInfo()
    {
        cout << "姓名：" << name << "，年龄：" << age << endl;
        cout << "学号：" << getStudentId() << endl;
        cout << "研究课题：" << researchTopic << endl;
    }
};

int main()
{
    // 创建教师对象
    Teacher t("110101199001011234", "张老师", 35, "T001", "计算机学院");

    // 创建学生对象
    Student s("110101200201011235", "李同学", 20, "S2021001");

    // 创建研究生对象
    GraduateStudent g("110101199901011236", "王同学", 24, "S2019001", "人工智能");

    // 测试1：教师信息访问
    t.displayTeacherInfo();
    // t.name = "张教授";  // 这行应该报错，为什么？

    // 测试2：学生信息访问
    s.displayStudentInfo();
    // s.age = 21;  // 这行应该报错，为什么？

    // 测试3：研究生信息访问
    g.displayGraduateInfo();
    // g.displayStudentInfo();  // 这行应该报错，为什么？

    return 0;
}
// @pintia code=end