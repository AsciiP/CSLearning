// 第7个实验：虚拟继承
// 目的：验证虚拟继承在多基类继承中的作用
// 要求：合理设计一个至少三层的类族，通过虚拟继承，解决基类成员访问冲突的问题。

#include <iostream>
using namespace std;

// 基类
class Base
{
public:
    void show()
    {
        cout << "Base class method" << endl;
    }
};

// 中间类1，虚拟继承
class Derived1 : virtual public Base
{
};

// 中间类2，虚拟继承
class Derived2 : virtual public Base
{
};

// 最终派生类
class FinalDerived : public Derived1, public Derived2
{
};

int main()
{
    FinalDerived obj;
    obj.show(); // 通过虚拟继承，避免了二义性
    system("pause");
    return 0;
}