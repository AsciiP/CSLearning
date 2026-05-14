/*
  @pintia psid=2051915249377234944 pid=2051915249389817856 compiler=GXX
  ProblemSet: 实验一 单链表操作
  Title: 单链表基本操作
  https://pintia.cn/problem-sets/2051915249377234944/exam/problems/type/7?problemSetProblemId=2051915249389817856
*/
// @pintia code=start
#include <iostream>
using namespace std;

struct ListNode
{
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(nullptr) {}
};

void insertNode(ListNode *&head, int k, int d)
{
    if (k < 0)
        return;

    if (k == 0)
    {
        ListNode *newNode = new ListNode(d);
        newNode->next = head;
        head = newNode;
        return;
    }

    ListNode *cur = head;
    for (int i = 1; i < k; ++i)
    {
        if (cur == nullptr)
            return;
        cur = cur->next;
    }

    if (cur == nullptr)
        return;

    ListNode *newNode = new ListNode(d);
    newNode->next = cur->next;
    cur->next = newNode;
}

void deleteNode(ListNode *&head, int k)
{
    if (k <= 0 || head == nullptr)
        return;

    if (k == 1)
    {

        ListNode *temp = head;
        head = head->next;
        delete temp;
        return;
    }

    ListNode *cur = head;
    for (int i = 1; i < k - 1; ++i)
    {
        if (cur == nullptr || cur->next == nullptr)
            return;
        cur = cur->next;
    }

    if (cur->next == nullptr)
        return;

    ListNode *temp = cur->next;
    cur->next = temp->next;
    delete temp;
}

void printList(ListNode *head)
{
    ListNode *cur = head;
    while (cur != nullptr)
    {
        cout << cur->val << " ";
        cur = cur->next;
    }
    cout << endl;
}

int main()
{
    int n;
    if (!(cin >> n))
        return 0;

    ListNode *head = nullptr;
    ListNode *tail = nullptr;
    for (int i = 0; i < n; ++i)
    {
        int val;
        cin >> val;
        if (head == nullptr)
        {
            head = new ListNode(val);
            tail = head;
        }
        else
        {
            tail->next = new ListNode(val);
            tail = tail->next;
        }
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i)
    {
        int op;
        cin >> op;
        if (op == 0)
        {
            // 插入
            int k, d;
            cin >> k >> d;
            insertNode(head, k, d);
        }
        else if (op == 1)
        {
            // 删除
            int k;
            cin >> k;
            deleteNode(head, k);
        }
    }

    printList(head);

    while (head != nullptr)
    {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }

    return 0;
}
// @pintia code=end