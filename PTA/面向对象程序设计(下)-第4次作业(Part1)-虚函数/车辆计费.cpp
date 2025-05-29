/*
  @pintia psid=1920807589209583616 pid=1920807589230555141 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第4次作业(Part1)-虚函数
  Title: 车辆计费
  https://pintia.cn/problem-sets/1920807589209583616/exam/problems/type/7?problemSetProblemId=1920807589230555141
*/
// @pintia code=start
#include <iostream>
#include <string>
using namespace std;

class Vehicle
{
protected:
    string NO;

public:
    Vehicle(string no) : NO(no) {}
    virtual void display() = 0;
    virtual ~Vehicle() {}
};

class Car : public Vehicle
{
    int passenger, weight;

public:
    Car(string no, int p, int w) : Vehicle(no), passenger(p), weight(w) {}
    void display() override
    {
        cout << NO << " " << passenger * 8 + weight * 2 << endl;
    }
};

class Truck : public Vehicle
{
    int weight;

public:
    Truck(string no, int w) : Vehicle(no), weight(w) {}
    void display() override
    {
        cout << NO << " " << weight * 5 << endl;
    }
};

class Bus : public Vehicle
{
    int passenger;

public:
    Bus(string no, int p) : Vehicle(no), passenger(p) {}
    void display() override
    {
        cout << NO << " " << passenger * 3 << endl;
    }
};

int main()
{
    Vehicle *pv[10];
    int cnt = 0;
    int type;
    while (cin >> type && type != 0)
    {
        string no;
        if (type == 1)
        {
            int p, w;
            cin >> no >> p >> w;
            pv[cnt++] = new Car(no, p, w);
        }
        else if (type == 2)
        {
            int w;
            cin >> no >> w;
            pv[cnt++] = new Truck(no, w);
        }
        else if (type == 3)
        {
            int p;
            cin >> no >> p;
            pv[cnt++] = new Bus(no, p);
        }
    }
    for (int i = 0; i < cnt; ++i)
    {
        pv[i]->display();
        delete pv[i];
    }
    return 0;
}
// @pintia code=end