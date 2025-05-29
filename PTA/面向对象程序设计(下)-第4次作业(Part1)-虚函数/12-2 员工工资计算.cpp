/*
  @pintia psid=1920807589209583616 pid=1920807589230555137 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part1)-虚函数
  Title: 12-2 员工工资计算
  https://pintia.cn/problem-sets/1920807589209583616/exam/problems/type/6?problemSetProblemId=1920807589230555137
*/
// @pintia code=start
#include <iostream>
using namespace std;

class Employee
{
public:
    int id;
    Employee() : id(0) {}
    Employee(int id_) : id(id_) {}
    virtual double calculateSalary() { return 0.0; }
    virtual void input()
    {
        cin >> id;
    }
    virtual ~Employee() {}
};

class Manager : public Employee
{
public:
    double baseSalary;
    double bonus;
    Manager() : Employee(), baseSalary(0.0), bonus(0.0) {}
    Manager(int id_, double baseSalary_, double bonus_)
        : Employee(id_), baseSalary(baseSalary_), bonus(bonus_) {}
    double calculateSalary() override
    {
        return baseSalary + bonus;
    }
    void input() override
    {
        cin >> id >> baseSalary >> bonus;
    }
    ~Manager() override {}
};
// @pintia code=end