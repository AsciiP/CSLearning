/*
  @pintia psid=2051922468713807872 pid=2051922468726390784 compiler=GXX
  ProblemSet: 实验二 基于循环链表的约瑟夫环的实现
  Title: 约瑟夫环
  https://pintia.cn/problem-sets/2051922468713807872/exam/problems/type/7?problemSetProblemId=2051922468726390784
*/
// @pintia code=start
#include <iostream>

using namespace std;

struct Node {
    int data;
    Node* next;
};

int main() {
    int N, p;
    cin >> N >> p;
    
    // Create circular llist
    Node* head = new Node();
    head->data = 1;
    head->next = head;
    
    Node* current = head;
    for (int i = 2; i <= N; i++) {
        Node* newNode = new Node();
        newNode->data = i;
        current->next = newNode;
        newNode->next = head;
        current = newNode;
    }
    
    Node* prev = current;
    current = head;
    bool first = true;
    
    while (current->next != current) {
        // Count p
        for (int i = 1; i < p; i++) {
            prev = current;
            current = current->next;
        }
        
        // Output the eliminated one
        if (!first) cout << " ";
        cout << current->data;
        first = false;
        
        // Remove current node from the list
        prev->next = current->next;
        Node* toDelete = current;
        current = current->next;
        delete toDelete;
    }
    
    if (!first) cout << " " << current->data << endl;
    else cout << current->data << endl;
    delete current;
    
    return 0;
}
// @pintia code=end