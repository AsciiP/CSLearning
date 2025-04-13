#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>

class BigInt
{
private:
    std::string value;
    bool isNegative;

    void removeLeadingZeros()
    {
        while (value.size() > 1 && value[0] == '0')
        {
            value.erase(value.begin());
        }
        if (value == "0")
        {
            isNegative = false;
        }
    }

    bool absLess(const BigInt &a, const BigInt &b) const
    {
        if (a.value.size() != b.value.size())
        {
            return a.value.size() < b.value.size();
        }
        return a.value < b.value;
    }

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

    std::string subtractStrings(const std::string &a, const std::string &b) const
    {
        std::string result;
        int borrow = 0, diff = 0;
        int i = a.size() - 1, j = b.size() - 1;

        while (i >= 0)
        {
            diff = a[i--] - '0' - borrow;
            if (j >= 0)
                diff -= b[j--] - '0';
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
        }
        std::reverse(result.begin(), result.end());
        return result;
    }

public:
    BigInt() : value("0"), isNegative(false) {}
    BigInt(const std::string &str) { *this = str; } // Use operator= to assign
    BigInt(int num) { *this = std::to_string(num); }
    BigInt(float num) { *this = std::to_string(int(num)); }
    BigInt(double num) { *this = std::to_string(int(num)); }

    BigInt &operator=(const std::string &str)
    {
        if (str.empty())
        {
            value = "0";
            isNegative = false;
        }
        else
        {
            isNegative = (str[0] == '-');
            value = isNegative ? str.substr(1) : str;
            removeLeadingZeros();
        }
        return *this;
    }

    BigInt operator+(const BigInt &other) const
    {
        if (isNegative == other.isNegative) // Same signs
        {
            BigInt result;
            result.value = addStrings(value, other.value);
            result.isNegative = isNegative;
            return result;
        }
        else // Different signs
        {
            if (absLess(*this, other))
            {
                BigInt result;
                result.value = subtractStrings(other.value, value);
                result.isNegative = other.isNegative;
                return result;
            }
            else
            {
                BigInt result;
                result.value = subtractStrings(value, other.value);
                result.isNegative = isNegative;
                return result;
            }
        }
    }

    BigInt operator-(const BigInt &other) const
    {
        BigInt negOther = other;
        negOther.isNegative = !other.isNegative;
        return *this + negOther;
    }

    BigInt operator*(const BigInt &other) const
    {
        BigInt result;
        result.value = "0";
        result.isNegative = (isNegative != other.isNegative);

        for (int i = other.value.size() - 1; i >= 0; --i)
        {
            std::string temp(value.size() + other.value.size(), '0');
            int carry = 0, product = 0;
            for (int j = value.size() - 1; j >= 0; --j)
            {
                product = (value[j] - '0') * (other.value[i] - '0') + carry;
                temp[j + i + 1] = product % 10 + '0';
                carry = product / 10;
            }
            temp[i] += carry;
            result.value = addStrings(result.value, temp);
        }
        result.removeLeadingZeros();
        return result;
    }

    BigInt operator/(const BigInt &other) const
    {
        // TODO
    }

    friend std::istream &operator>>(std::istream &in, BigInt &bigInt)
    {
        std::string input;
        in >> input;
        bigInt = input;
        return in;
    }

    friend std::ostream &operator<<(std::ostream &out, const BigInt &bigInt)
    {
        if (bigInt.isNegative)
            out << '-';
        out << bigInt.value;
        return out;
    }
};

int main()
{
    BigInt a, b;
    std::cout << "Enter two big integers:" << std::endl;
    std::cin >> a >> b;

    std::cout << "a + b = " << a + b << std::endl;
    std::cout << "a - b = " << a - b << std::endl;
    std::cout << "a * b = " << a * b << std::endl;
    std::cout << "a / b = " << a / b << std::endl;

    return 0;
}