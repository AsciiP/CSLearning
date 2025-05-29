/*
    @pintia psid=1920807589209583616 pid=1920807589230555139 compiler=GXX
    ProblemSet: 面向对象程序设计(下)-第4次作业(Part1)-虚函数
    Title: 12-4 员工工资计算II
    https://pintia.cn/problem-sets/1920807589209583616/exam/problems/type/6?problemSetProblemId=1920807589230555139
*/
// @pintia code=start
#include <iostream>
using namespace std;

class Employee
{
public:
    int id;
    Employee(int id = 0) : id(id) {}
    virtual double calculateSalary() = 0;
    virtual void input() = 0;
    virtual ~Employee() {}
};

class Manager : public Employee
{
protected:
    double baseSalary;
    double bonus;

public:
    Manager() : Employee(), baseSalary(0), bonus(0) {}
    Manager(int id, double baseSalary, double bonus)
        : Employee(id), baseSalary(baseSalary), bonus(bonus) {}
    double calculateSalary() override
    {
        return baseSalary + bonus;
    }
    void input() override
    {
        cout << "Enter ID:" << endl;
        cin >> id;
        cout << "Enter base salary:" << endl;
        cin >> baseSalary;
        cout << "Enter bonus:" << endl;
        cin >> bonus;
    }
    ~Manager() override {}
};

class Programmer : public Employee
{
protected:
    double hourlyRate;
    double hoursWorked;

public:
    Programmer() : Employee(), hourlyRate(0), hoursWorked(0) {}
    Programmer(int id, double hourlyRate, double hoursWorked)
        : Employee(id), hourlyRate(hourlyRate), hoursWorked(hoursWorked) {}
    double calculateSalary() override
    {
        return hourlyRate * hoursWorked;
    }
    void input() override
    {
        cout << "Enter ID:" << endl;
        cin >> id;
        cout << "Enter hourly rate:" << endl;
        cin >> hourlyRate;
        cout << "Enter hours worked:" << endl;
        cin >> hoursWorked;
    }
    ~Programmer() override {}
};

class Tester : public Employee
{
protected:
    double baseSalary;
    double overtimePay;

public:
    Tester() : Employee(), baseSalary(0), overtimePay(0) {}
    Tester(int id, double baseSalary, double overtimePay)
        : Employee(id), baseSalary(baseSalary), overtimePay(overtimePay) {}
    double calculateSalary() override
    {
        return baseSalary + overtimePay;
    }
    void input() override
    {
        cout << "Enter ID:" << endl;
        cin >> id;
        cout << "Enter base salary:" << endl;
        cin >> baseSalary;
        cout << "Enter overtime pay:" << endl;
        cin >> overtimePay;
    }
    ~Tester() override {}
};
// @pintia code=end