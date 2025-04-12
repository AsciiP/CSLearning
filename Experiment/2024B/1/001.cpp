// 第1个实验：用数组实现线性表。
// 目的：练习类的设计和使用。体会抽象数据类型的内涵。
// 要求：设计和实现一个线性表类SList，满足：
// 1）用数组实现存储；
// 2）实现线性表的基本操作；
// 3）测试代码验证 List的基本操作。

#include <iostream>

class SList
{
private:
    int *data;
    int maxSize;
    int currentSize;
public:
    SList(int size)
    {
        maxSize = size;
        currentSize = 0;
        data = new int[maxSize];
    }
    SList(const SList &list)
    {
        maxSize = list.maxSize;
        currentSize = list.currentSize;
        data = new int[maxSize];
        for (int i = 0; i < currentSize; i++)
        {
            data[i] = list.data[i];
        }
    }
    ~SList()
    {
        delete[] data;
    }
    void insert(int index, int value)
    {
        // Range check
        if (index < 0 || index > currentSize)
        {
            std::cout << "Index out of range." << std::endl;
            return;
        }
        // Check if the list is full
        if (currentSize == maxSize)
        {
            std::cout << "List is full." << std::endl;
            return;
        }
        // Move elements
        for (int i = currentSize; i > index; i--)
        {
            data[i] = data[i - 1];
        }
        // Insert value
        data[index] = value;
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
        // Move elements
        for (int i = index; i < currentSize - 1; i++)
        {
            data[i] = data[i + 1];
        }
        currentSize--;
    }
    void display()
    {
        for (int i = 0; i < currentSize; i++)
        {
            std::cout << data[i] << " ";
        }
        std::cout << std::endl;
    }
    void reverse()
    {
        for (int i = 0; i < currentSize / 2; i++)
        {
            int temp = data[i];
            data[i] = data[currentSize - i - 1];
            data[currentSize - i - 1] = temp;
        }
    }
    const int &operator[](int index) const
    {
        return data[index];
    }
    friend std::ostream &operator<<(std::ostream &os, const SList &list)
    {
        for (int i = 0; i < list.currentSize; i++)
        {
            os << list.data[i] << " ";
        }
        return os;
    }
    friend std::istream &operator>>(std::istream &is, SList &list)
    {
        for (int i = 0; i < list.maxSize; i++)
        {
            is >> list.data[i];
        }
        list.currentSize = list.maxSize;
        return is;
    }
    const int size() const
    {
        return currentSize;
    }
    const int capacity() const
    {
        return maxSize;
    }
    const int find(int value) const
    {
        for (int i = 0; i < currentSize; i++)
        {
            if (data[i] == value)
            {
                return i;
            }
        }
        return -1;
    }
};

int main()
{
    SList list(10);                             // Create a list with size 10
    std::cin >> list;                           // Input 10 integers
    std::cout << list << std::endl;             // Display the list
    list.reverse();                             // Reverse the list
    list.remove(5);                             // Remove the 6th element
    std::cout << list << std::endl;             // Display the list
    list.insert(5, 100);                        // Insert 100 at the 6th position
    std::cout << list << std::endl;             // Display the list
    std::cout << list[5] << std::endl;          // Display the 6th element
    std::cout << list.size() << std::endl;      // Display the size of the list
    std::cout << list.capacity() << std::endl;  // Display the capacity of the list
    std::cout << list.find(100) << std::endl;   // Find the index of 100
    return 0;
}