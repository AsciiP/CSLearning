/*
  @pintia psid=1903955893807046656 pid=1903955893832212489 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 设计一个People类
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212489
*/
// @pintia code=start
#include <iostream>

class People
{
private:
    std::string name;
    int age;
    double height;
    double weight;
    static int count;

public:
    People(std::string name = "", int age = 0, double height = 0, double weight = 0)
        : name(name), age(age), height(height), weight(weight)
    {
        count++;
    }

    void display() const
    {
        std::cout << "Name: " << name << ", Age: " << age << ", Height: " << height << ", Weight: " << weight << std::endl;
    }

    static int getCount()
    {
        return count;
    }
};

int People::count = 0;

int main()
{
    std::string name;
    int age;
    double height, weight;

    while (std::cin >> name && name != "exit") {
        std::cin >> age >> height >> weight;
        People person(name, age, height, weight);
    }

    std::cout << People::getCount() << std::endl;
}
// @pintia code=end