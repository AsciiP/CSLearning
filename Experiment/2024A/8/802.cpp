#include <iostream>

using namespace std;

class Node
{
public:
    int data;
    Node *next;
};

Node *create()
{
    Node *head = nullptr;
    Node *tail = nullptr;

    int value;
    while (cin >> value && value != -1)
    {
        Node *currentNode = new Node;
        currentNode->data = value;
        currentNode->next = nullptr;

        if (head == nullptr)
        {
            head = currentNode;
            tail = currentNode;
        }
        else
        {
            tail->next = currentNode;
            tail = currentNode;
        }
    }
    return head;
}

void printList(Node *head)
{
    Node *currentVisited = head;
    while (currentVisited != nullptr)
    {
        cout << currentVisited->data << " ";
        currentVisited = currentVisited->next;
    }
    cout << endl;
}

void freeList(Node *head)
{
    Node *currentVisited = head;
    while (currentVisited != nullptr)
    {
        Node *nextNode = currentVisited->next;
        delete currentVisited;
        currentVisited = nextNode;
    }
}

int main()
{
    Node *linkedList = create();

    printList(linkedList);

    freeList(linkedList);

    return 0;
}
