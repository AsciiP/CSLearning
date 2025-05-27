#include <iostream>
#include <string>
using namespace std;
class Ball
{
protected:
    string opponent;
    int sore;

public:
    Ball(string A)
    {
        opponent = A;
    }
    void display()
    {
        cout << opponent << " " << sore << endl;
    }
};
class Basketball : public Ball
{
public:
    Basketball(string name, int A, int B) : Ball(name)
    {
        if (A > B)
            sore = 2;
        if (A < B)
            sore = 1;
        if (A == B)
            sore = 0;
    }
};
class Football : public Ball
{
public:
    Football(string name, int A, int B) : Ball(name)
    {
        if (A > B)
            sore = 3;
        if (A < B)
            sore = 0;
        if (A == B)
            sore = 1;
    }
};
class Volleyball : public Ball
{
public:
    Volleyball(string name, int A, int B) : Ball(name)
    {
        if (A == 3 && (B == 0 || B == 1))
            sore = 3;
        else if (A == 3 && B == 2)
            sore = 2;
        else if (A == 2 && B == 3)
            sore = 1;
        else
            sore = 0;
    }
};
int main()
{
    int type;
    int count = -1;
    string name;
    int A, B;
    Ball *pb[20];
    while (cin >> type, type)
    {
        count++;
        cin >> name;
        cin >> A >> B;
        if (type == 1)
        {
            pb[count] = new Basketball(name, A, B);
            pb[count]->display();
        }
        else if (type == 2)
        {
            pb[count] = new Football(name, A, B);
            pb[count]->display();
        }
        else if (type == 3)
        {
            pb[count] = new Volleyball(name, A, B);
            pb[count]->display();
        }
        else
            cerr << "type mistake";
    }
    for (int i = 0; i <= count; i++)
    {
        delete pb[i];
    }
}