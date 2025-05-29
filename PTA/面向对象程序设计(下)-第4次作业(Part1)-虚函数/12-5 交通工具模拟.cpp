/*
  @pintia psid=1920807589209583616 pid=1920807589230555142 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part1)-虚函数
  Title: 12-5 交通工具模拟
  https://pintia.cn/problem-sets/1920807589209583616/exam/problems/type/7?problemSetProblemId=1920807589230555142
*/
// @pintia code=start
#include <iostream>
using namespace std;

class Vehicle
{
public:
    virtual void move() = 0;
    virtual void input() = 0;
    virtual ~Vehicle() {}
};

class Car : public Vehicle
{
private:
    int speed;

public:
    Car() : speed(0) {}
    void move() override
    {
        cout << "Car moves on roads at speed " << speed << " km/h." << endl;
    }
    void input() override
    {
        cout << "Enter car speed (km/h):";
        cin >> speed;
    }
    ~Car() override {}
};

class Bicycle : public Vehicle
{
private:
    int speed;

public:
    Bicycle() : speed(0) {}
    void move() override
    {
        cout << "Bicycle moves on roads at speed " << speed << " km/h." << endl;
    }
    void input() override
    {
        cout << "Enter bicycle speed (km/h):";
        cin >> speed;
    }
    ~Bicycle() override {}
};

class Airplane : public Vehicle
{
private:
    int altitude;

public:
    Airplane() : altitude(0) {}
    void move() override
    {
        cout << "Airplane flies in the sky at altitude " << altitude << " meters." << endl;
    }
    void input() override
    {
        cout << "Enter airplane altitude (meters):";
        cin >> altitude;
    }
    ~Airplane() override {}
};

int main() {
    Vehicle* vehicles[3];
    vehicles[0] = new Car();
    vehicles[1] = new Bicycle();
    vehicles[2] = new Airplane();

    for (int i = 0; i < 3; ++i) {
        cout << "Input for Vehicle " << i + 1 << ":" << endl;
        vehicles[i]->input();
    }

    for (int i = 0; i < 3; ++i) {
        cout << "Vehicle " << i + 1 << ": ";
        vehicles[i]->move();
    }

    // 释放内存
    for (int i = 0; i < 3; ++i) {
        delete vehicles[i];
    }

    return 0;
}
// @pintia code=end