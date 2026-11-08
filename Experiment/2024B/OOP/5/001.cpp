// 第5个实验：通过继承实现栈。
// 目的：练习类的继承（派生）。
// 要求：以DList为基类，派生栈类：DStack：
// 1）实现栈的基本操作；
// 2）编写测试代码验证栈的功能。

#include <iostream>

class DList
{
private:
    struct Node
    {
        int data;
        Node *next;
    };
    Node *head;
    Node *tail;
    int currentSize;
public:
    DList()
    {
        head = nullptr;
        tail = nullptr;
        currentSize = 0;
    }
    DList(const DList &list)
    {
        head = nullptr;
        tail = nullptr;
        currentSize = 0;
        Node *current = list.head;
        while (current != nullptr)
        {
            insert(currentSize, current->data);
            current = current->next;
        }
    }
    ~DList()
    {
        Node *current = head;
        while (current != nullptr)
        {
            Node *temp = current;
            current = current->next;
            delete temp;
        }
    }
    void insert(int index, int value)
    {
        // Range check
        if (index < 0 || index > currentSize)
        {
            std::cout << "Index out of range." << std::endl;
            return;
        }
        // Create a new node
        Node *newNode = new Node;
        newNode->data = value;
        // Insert at the beginning
        if (index == 0)
        {
            newNode->next = head;
            head = newNode;
            if (currentSize == 0)
            {
                tail = newNode;
            }
        }
        // Insert at the end
        else if (index == currentSize)
        {
            tail->next = newNode;
            tail = newNode;
            tail->next = nullptr;
        }
        // Insert in the middle
        else
        {
            Node *current = head;
            for (int i = 0; i < index - 1; i++)
            {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        currentSize++;
    }
    void remove(int index)
    {
        // Range check
        if (index < 0 || index >= currentSize)
        {
            std::cout << "Index out of range." << std::endl;
            return;
        }
        // Remove the first node
        if (index == 0)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
            if (currentSize == 1)
            {
                tail = nullptr;
            }
        }
        // Remove the last node
        else if (index == currentSize - 1)
        {
            Node *current = head;
            for (int i = 0; i < index - 1; i++)
            {
                current = current->next;
            }
            Node *temp = tail;
            tail = current;
            tail->next = nullptr;
            delete temp;
        }
        // Remove in the middle
        else
        {
            Node *current = head;
            for (int i = 0; i < index - 1; i++)
            {
                current = current->next;
            }
            Node *temp = current->next;
            current->next = current->next->next;
            delete temp;
        }
        currentSize--;
    }
    void display()
    {
        Node *current = head;
        while (current != nullptr)
        {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
    void reverse()
    {
        Node *current = head;
        Node *prev = nullptr;
        Node *next = nullptr;
        while (current != nullptr)
        {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        head = prev;
    }
    int &operator[](int index) const
    {
        // Range check
        if (index < 0 || index >= currentSize)
        {
            std::cout << "Index out of range." << std::endl;
        }
        Node *current = head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current->data;
    }
    friend std::ostream &operator<<(std::ostream &os, const DList &list)
    {
        Node *current = list.head;
        while (current != nullptr)
        {
            os << current->data << " ";
            current = current->next;
        }
        return os;
    }
    friend std::istream &operator>>(std::istream &is, DList &list)
    {
        int value;
        while (is >> value)
        {
            list.insert(list.currentSize, value);
        }
        return is;
    }
    const int size() const
    {
        return currentSize;
    }
    const int find(int value) const
    {
        Node *current = head;
        for (int i = 0; i < currentSize; i++)
        {
            if (current->data == value)
            {
                return i;
            }
            current = current->next;
        }
        return -1;
    }
};

class DStack : public DList
{
public:
    DStack() : DList() {}
    DStack(const DStack &stack) : DList(stack) {}
    ~DStack() {}

    void push(int value)
    {
        insert(0, value);
    }

    void pop()
    {
        remove(0);
    }

    int top() const
    {
        return (*this)[0];
    }
};

int main()
{
    DStack stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    std::cout << "Stack: " << stack << std::endl;
    std::cout << "Top element: " << stack.top() << std::endl;
    stack.pop();
    std::cout << "After pop, Stack: " << stack << std::endl;

    return 0;
}