/*
  @pintia psid=2051923289388244992 pid=2051923289405022208 compiler=GXX
  ProblemSet: 实验三-栈的应用之表达式转换
  Title: 表达式转换
  https://pintia.cn/problem-sets/2051923289388244992/exam/problems/type/7?problemSetProblemId=2051923289405022208
*/
// @pintia code=start
#include <iostream>
#include <stack>
#include <string>

using namespace std;

bool isNumber(char c)
{
    return ((c >= '0' && c <= '9') || c == '.');
}

int precedence(char op)
{
    if (op == '+' || op == '-')
        return 1;
    if (op == '*' || op == '/')
        return 2;
    return 0;
}

string infixToPostfix(const string &infix)
{
    stack<char> opStack;
    string postfix;

    bool lastWasNumber = false;
    bool negative = false;

    for (char c : infix)
    {
        if (isNumber(c))
        {
            if (!lastWasNumber && !postfix.empty())
                postfix += ' ';
            if (negative)
            {
                postfix += '-';
                negative = false;
            }
            postfix += c;
            lastWasNumber = true;
        }
        else if (c == '(')
        {
            opStack.push(c);
            lastWasNumber = false;
        }
        else if (c == ')')
        {
            while (!opStack.empty() && opStack.top() != '(')
            {
                postfix += ' ';
                postfix += opStack.top();
                opStack.pop();
            }
            if (!opStack.empty())
                opStack.pop();
            lastWasNumber = true;
        }
        else if ((c == '+' || c == '-') && !lastWasNumber)
        {
            if (c == '-')
                negative = true;
        }
        else
        {
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(c))
            {
                postfix += ' ';
                postfix += opStack.top();
                opStack.pop();
            }
            opStack.push(c);
            lastWasNumber = false;
        }
    }

    while (!opStack.empty())
    {
        postfix += ' ';
        postfix += opStack.top();
        opStack.pop();
    }

    return postfix;
}

int main()
{
    string infix;
    cin >> infix;
    string postfix = infixToPostfix(infix);
    for (char c : postfix)
    {
        cout << c;
    }
    return 0;
}
// @pintia code=end