// 第6个实验：虚函数。
// 目的：练习虚函数的使用方法，体会虚函数的作用。
// 要求：以DList为基类，派生队列类：DQueue。结合上次实验的DStack，完成：
// 1）在DList中增加与插入位置无关的纯虚函数Insert()和Delete()；
// 1）由用户指定一种数据结构类型；
// 2）根据用户指定的数据结构类型动态创建对应的数据结构；
// 3）分别由Insert和Delete对应入栈和出栈或者入队和出队操作。
// 4）由测试代码验证以上操作。

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

protected:
    void insertAtEnd(int value)
    {
        Node *newNode = new Node{value, nullptr};
        if (tail)
        {
            tail->next = newNode;
        }
        else
        {
            head = newNode;
        }
        tail = newNode;
        currentSize++;
    }

    void removeFromFront()
    {
        if (head)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
            currentSize--;
            if (!head)
            {
                tail = nullptr;
            }
        }
    }

    void removeFromEnd()
    {
        if (head)
        {
            if (head == tail)
            {
                delete head;
                head = tail = nullptr;
            }
            else
            {
                Node *current = head;
                while (current->next != tail)
                {
                    current = current->next;
                }
                delete tail;
                tail = current;
                tail->next = nullptr;
            }
            currentSize--;
        }
    }

public:
    DList() : head(nullptr), tail(nullptr), currentSize(0) {}
    virtual ~DList()
    {
        while (head)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
    }

    virtual void Insert(int value) = 0;
    virtual void Delete() = 0;

    void display() const
    {
        Node *current = head;
        while (current)
        {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

// Derived class for stack
class DStack : public DList
{
public:
    void Insert(int value) override
    {
        insertAtEnd(value);
    }

    void Delete() override
    {
        removeFromEnd();
    }
};

class DQueue : public DList
{
public:
    void Insert(int value) override
    {
        insertAtEnd(value);
    }

    void Delete() override
    {
        removeFromFront();
    }
};

int main()
{
    DList *dataStructure = nullptr;
    int choice;

    std::cout << "Choose data structure type:\n";
    std::cout << "1. Stack\n";
    std::cout << "2. Queue\n";
    std::cin >> choice;

    if (choice == 1)
    {
        dataStructure = new DStack();
    }
    else if (choice == 2)
    {
        dataStructure = new DQueue();
    }
    else
    {
        std::cout << "Invalid choice!" << std::endl;
        return 1;
    }

    int operation, value;
    do
    {
        std::cout << "\nChoose operation:\n";
        std::cout << "1. Insert\n";
        std::cout << "2. Delete\n";
        std::cout << "3. Display\n";
        std::cout << "4. Exit\n";
        std::cin >> operation;

        switch (operation)
        {
        case 1:
            std::cout << "Enter value to insert: ";
            std::cin >> value;
            dataStructure->Insert(value);
            break;
        case 2:
            dataStructure->Delete();
            break;
        case 3:
            std::cout << "Current structure: ";
            dataStructure->display();
            break;
        case 4:
            std::cout << "Exiting..." << std::endl;
            break;
        default:
            std::cout << "Invalid operation!" << std::endl;
        }
    } while (operation != 4);

    delete dataStructure;
    return 0;
}
