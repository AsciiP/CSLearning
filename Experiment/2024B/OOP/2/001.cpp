// 第2个实验：用链表实现线性表。
// 目的：练习类的构造和析构，了解他们在对象生命周期中的作用。
// 要求：设计和实现一个线性表类DList，满足：
// 1）用链表实现存储；
// 2）实现线性表的基本操作；
// 3）测试代码验证 List的基本操作。

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

int main()
{
    DList list;                                 // Create a list with size 10
    std::cin >> list;                           // Input 10 integers
    std::cout << list << std::endl;             // Display the list
    list.reverse();                             // Reverse the list
    list.remove(5);                             // Remove the 6th element
    std::cout << list << std::endl;             // Display the list
    list.insert(5, 100);                        // Insert 100 at the 6th position
    std::cout << list << std::endl;             // Display the list
    std::cout << list[5] << std::endl;          // Display the 6th element
    std::cout << list.size() << std::endl;      // Display the size of the list
    std::cout << list.find(100) << std::endl;   // Find the index of 100
    return 0;
}