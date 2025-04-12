#include <iostream>
using std::cout, std::cin, std::endl;
using std::istream, std::ostream;

#include <iomanip>
using std::setw;

class Array
{
    friend istream &operator>>(istream &input, Array &a)
    {
        for (int i = 0; i < a.size; i++)
        {
            input >> a.ptr[i];
        }
        return input;
    }
    friend ostream &operator<<(ostream &output, const Array &a)
    {
        int i;
        for (int i = 0; i < a.size; i++)
        {
            output << setw(12) << a.ptr[i];
            if ((i + 1) % 4 == 0)
            {
                output << endl;
            }
        }
        output << endl;
        return output;
    }

public:
    Array(int = 10);
    Array(const Array &);
    ~Array();
    int getSize() const;

    const Array &operator=(const Array &);
    bool operator==(const Array &) const;
    bool operator!=(const Array &right) const
    {
        return !(*this == right);
    }
    int &operator[](int);
    const int &operator[](int) const;

private:
    int size;
    int *ptr;
};

Array::Array(int arraySize)
{
    size = (arraySize > 0 ? arraySize : 10);
    ptr = new int[size];
    for (int i = 0; i < size; i++)
    {
        ptr[i] = 0;
    }
}

Array::Array(const Array &arrayToCopy) : size(arrayToCopy.size)
{
    ptr = new int[size];
    for (int i = 0; i < size; i++)
    {
        ptr[i] = arrayToCopy.ptr[i];
    }
}

Array::~Array()
{
    delete[] ptr;
}

int Array::getSize() const
{
    return size;
}

const Array &Array::operator=(const Array &right)
{
    if (&right != this)
    {
        if (size != right.size)
        {
            delete[] ptr;
            size = right.size;
            ptr = new int[size];
        }
        for (int i = 0; i < size; i++)
        {
            ptr[i] = right.ptr[i];
        }
    }
    return *this;
}

bool Array::operator==(const Array &right) const
{
    if (size != right.size)
    {
        return false;
    }
    for (int i = 0; i < size; i++)
    {
        if (ptr[i] != right.ptr[i])
        {
            return false;
        }
    }
    return true;
}

int &Array::operator[](int subscript)
{
    if (subscript < 0 or subscript >= size)
    {
        cout << "Error";
    }
    return ptr[subscript];
}

const int &Array::operator[](int subscript) const
{
    if (subscript < 0 or subscript >= size)
    {
        cout << "Error";
    }
    return ptr[subscript];
}

int main()
{
    Array integers1(7);
    Array integers2;

    cout << "Size of array integers1 is "
         << integers1.getSize()
         << "\nArray after initialization:\n" << integers1;

    cout << "Size of array integers2 is "
         << integers2.getSize()
         << "\nArray after initialization:\n" << integers2;

    cout << "Input 17 integers:\n";
    cin >> integers1 >> integers2;

    cout << "\nAfter input, the arrays contain:\n"
         << "integers1:\n"
         << integers1
         << "integers2:\n"
         << integers2;

    cout << "Evaluating: integers1 != integers2" << endl;
    if (integers1 == integers2)
    {
        cout << "Equal" << endl;
    }
    else
    {
        cout << "Not equal" << endl;
    }
    
}