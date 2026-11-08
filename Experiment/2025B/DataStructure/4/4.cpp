/*
  @pintia psid=2051923485341790208 pid=2051923485354373120 compiler=GXX
  ProblemSet: 实验四-队列的应用之舞伴匹配问题
  Title: 舞伴配对问题
  https://pintia.cn/problem-sets/2051923485341790208/exam/problems/type/7?problemSetProblemId=2051923485354373120
*/
// @pintia code=start
#include <iostream>
#include <queue>
#include <string>

using namespace std;

typedef struct
{
    string name;
    char sex;
} Person;

int main()
{
    int n;
    cin >> n;
    queue<Person> maleQueue, femaleQueue;
    cout << "配好的舞伴为：" << endl;
    for (int i = 0; i < n; i++)
    {
        Person p;
        cin >> p.name >> p.sex;
        if (p.sex == 'M' || p.sex == 'm')
        {
            maleQueue.push(p);
        }
        else
        {
            femaleQueue.push(p);
        }

        if (!maleQueue.empty() && !femaleQueue.empty())
        {
            Person male = maleQueue.front();
            Person female = femaleQueue.front();
            cout << female.name << "  " << male.name << endl;
            maleQueue.pop();
            femaleQueue.pop();
        }
    }

    if (!maleQueue.empty())
    {
        cout << "未配好舞伴的第一位男士为：" << maleQueue.front().name << endl;
    }
    else if (!femaleQueue.empty())
    {
        cout << "未配好舞伴的第一位女士为：" << femaleQueue.front().name << endl;
    }
}
// @pintia code=end