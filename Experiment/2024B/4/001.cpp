// 第4个实验：大整数类。
// 目的：练习运算符重载的实现和具体应用，进一步练习类的设计和使用，练习对数据的操控能力。
// 要求：实现不超过100位的大整数类型，支持：
// 1）流输入，输出；
// 2）赋值；
// 3）加、减，乘，除运算；
// 4）与int，float，double的混合运算。

#include <iostream>
#include <string>
#include <algorithm>

class BigInt
{
private:
    std::string value;
    bool isNegative;

    // Helper function to compare two positive numbers
    bool compareStrings(const std::string &a, const std::string &b) const
    {
        if (a.size() != b.size())
            return a.size() < b.size();
        return a < b;
    }

    // Helper function to add two positive numbers
    std::string addStrings(const std::string &a, const std::string &b) const
    {
        std::string result;
        int carry = 0, sum = 0;
        int i = a.size() - 1, j = b.size() - 1;

        while (i >= 0 || j >= 0 || carry)
        {
            sum = carry;
            if (i >= 0)
                sum += a[i--] - '0';
            if (j >= 0)
                sum += b[j--] - '0';
            result.push_back(sum % 10 + '0');
            carry = sum / 10;
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    // Helper function to subtract two positive numbers (a >= b)
    std::string subtractStrings(const std::string &a, const std::string &b) const
    {
        std::string result;
        int borrow = 0, diff = 0;
        int i = a.size() - 1, j = b.size() - 1;

        while (i >= 0)
        {
            diff = (a[i] - '0') - (j >= 0 ? (b[j--] - '0') : 0) - borrow;
            if (diff < 0)
            {
                diff += 10;
                borrow = 1;
            }
            else
            {
                borrow = 0;
            }
            result.push_back(diff + '0');
            i--;
        }

        while (result.size() > 1 && result.back() == '0')
        {
            result.pop_back();
        }

        std::reverse(result.begin(), result.end());
        return result;
    }

    // Helper function to multiply two positive numbers
    std::string multiplyStrings(const std::string &a, const std::string &b) const
    {
        std::string result(a.size() + b.size(), '0');

        for (int i = a.size() - 1; i >= 0; --i)
        {
            int carry = 0;
            for (int j = b.size() - 1; j >= 0; --j)
            {
                int temp = (result[i + j + 1] - '0') + (a[i] - '0') * (b[j] - '0') + carry;
                result[i + j + 1] = temp % 10 + '0';
                carry = temp / 10;
            }
            result[i] += carry;
        }

        size_t startpos = result.find_first_not_of('0');
        if (startpos != std::string::npos)
        {
            return result.substr(startpos);
        }
        return "0";
    }

    // Helper function to divide two positive numbers
    std::pair<std::string, std::string> divideStrings(const std::string &a, const std::string &b) const
    {
        if (b == "0")
            throw std::invalid_argument("Division by zero");
        std::string quotient, remainder = "0";

        for (char digit : a)
        {
            remainder += digit;
            size_t count = 0;
            while (remainder.size() > 1 && remainder[0] == '0')
            {
                remainder.erase(0, 1);
            }
            while (!compareStrings(remainder, b)) // remainder >= b
            {
                remainder = subtractStrings(remainder, b);
                count++;
            }
            quotient += (count + '0');
        }

        size_t startpos = quotient.find_first_not_of('0');
        if (startpos != std::string::npos)
        {
            quotient = quotient.substr(startpos);
        }
        else
        {
            quotient = "0";
        }

        return {quotient, remainder};
    }

public:
    BigInt() : value("0"), isNegative(false) {}

    BigInt(const std::string &val)
    {
        if (val.empty())
        {
            value = "0";
            isNegative = false;
        }
        else
        {
            isNegative = (val[0] == '-');
            value = isNegative ? val.substr(1) : val;
            if (value.empty() || value == "0")
            {
                value = "0";
                isNegative = false;
            }
        }
    }

    BigInt(const BigInt &other) : value(other.value), isNegative(other.isNegative) {}

    BigInt &operator=(const BigInt &other)
    {
        if (this != &other)
        {
            value = other.value;
            isNegative = other.isNegative;
        }
        return *this;
    }

    BigInt operator+(const BigInt &other) const
    {
        if (isNegative == other.isNegative)
        {
            return BigInt((isNegative ? "-" : "") + addStrings(value, other.value));
        }

        // The sign of the result depends on the larger absolute value
        bool resultNegative = false;
        std::string resultValue;
        if (compareStrings(value, other.value))
        {
            resultValue = subtractStrings(other.value, value);
            resultNegative = other.isNegative;
        }
        else
        {
            resultValue = subtractStrings(value, other.value);
            resultNegative = isNegative;
        }

        return BigInt((resultNegative && resultValue != "0" ? "-" : "") + resultValue);
    }

    BigInt operator-(const BigInt &other) const
    {
        return *this + BigInt((other.isNegative ? "" : "-") + other.value);
    }

    BigInt operator*(const BigInt &other) const
    {
        return BigInt((isNegative != other.isNegative ? "-" : "") + multiplyStrings(value, other.value));
    }

    BigInt operator/(const BigInt &other) const
    {
        auto result = divideStrings(value, other.value);
        return BigInt((isNegative != other.isNegative ? "-" : "") + result.first);
    }

    BigInt operator%(const BigInt &other) const
    {
        auto result = divideStrings(value, other.value);
        return BigInt(result.second);
    }

    friend std::ostream &operator<<(std::ostream &os, const BigInt &bigInt)
    {
        if (bigInt.isNegative && bigInt.value != "0")
            os << "-";
        os << bigInt.value;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, BigInt &bigInt)
    {
        std::string input;
        is >> input;
        bigInt = BigInt(input);
        return is;
    }
};

int main()
{
    std::cout << "Enter two large integers: ";
    BigInt a, b;
    std::cin >> a >> b;

    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << a - b << std::endl;
    std::cout << "a * b = " << a * b << std::endl;
    std::cout << "a / b = " << a / b << std::endl;
    std::cout << "a % b = " << a % b << std::endl;

    return 0;
}