/*
  @pintia psid=1910112809178632192 pid=1910112809203798022 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 动物世界
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798022
*/
// @pintia code=start
#include <iostream>
using namespace std;

enum COLOR
{
    WHITE,
    RED,
    BROWN,
    BLACK,
    KHAKI
};

class Mammal
{
public:
    // Constructors
    Mammal() : itsAge(0), itsWeight(0) {}
    Mammal(int age) : itsAge(age), itsWeight(0) {}
    ~Mammal() {}

    // Accessors
    int GetAge() const { return itsAge; }
    void SetAge(int age) { itsAge = age; }
    int GetWeight() const { return itsWeight; }
    void SetWeight(int weight) { itsWeight = weight; }

    // Other methods
    void Speak() const { cout << "Mammal is speaking..." << endl; }
    void Sleep() const { cout << "Mammal is sleeping..." << endl; }

protected:
    int itsAge;
    int itsWeight;
};

class Dog : public Mammal
{
public:
    // Constructors
    Dog() : Mammal(), itsColor(WHITE) {}
    Dog(int age) : Mammal(age), itsColor(WHITE) {}
    Dog(int age, int weight) : Mammal(age), itsColor(WHITE) { itsWeight = weight; }
    Dog(int age, COLOR color) : Mammal(age), itsColor(color) {}
    Dog(int age, int weight, COLOR color) : Mammal(age), itsColor(color) { itsWeight = weight; }
    ~Dog() {}

    // Accessors
    COLOR GetColor() const { return itsColor; }
    void SetColor(COLOR color) { itsColor = color; }

    // Other methods
    void WagTail() const { cout << "The dog is wagging its tail..." << endl; }
    void BegForFood() const { cout << "The dog is begging for food..." << endl; }

private:
    COLOR itsColor;
};

int main()
{
    Dog Fido;
    Dog Rover(5);
    Dog Buster(6, 8);
    Dog Yorkie(3, RED);
    Dog Dobbie(4, 20, KHAKI);

    Fido.Speak();
    Rover.WagTail();
    cout << "Yorkie is " << Yorkie.GetAge() << " years old." << endl;
    cout << "Dobbie weighs " << Dobbie.GetWeight() << " pounds." << endl;

    return 0;
}
// @pintia code=end