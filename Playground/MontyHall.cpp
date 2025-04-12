#include <iostream>
#include <cstdlib>
#include <ctime>

void 模拟蒙提霍尔(int 模拟次数, int& 换门赢, int& 不换门赢)
{
    for (int i = 0; i < 模拟次数; ++i)
    {
        int 汽车位置 = std::rand() % 3;
        int 初始选择 = std::rand() % 3;
        int 揭示门;

        do
        {
            揭示门 = std::rand() % 3;
        } while (揭示门 == 汽车位置 || 揭示门 == 初始选择);

        int 换门选择;
        for (int j = 0; j < 3; ++j)
        {
            if (j != 初始选择 && j != 揭示门)
            {
                换门选择 = j;
                break;
            }
        }

        if (换门选择 == 汽车位置)
        {
            ++换门赢;
        }
        else if (初始选择 == 汽车位置)
        {
            ++不换门赢;
        }
    }
}

int main()
{
    std::srand(std::time(0));
    const int 模拟次数 = 1000000;
    int 换门赢 = 0;
    int 不换门赢 = 0;

    模拟蒙提霍尔(模拟次数, 换门赢, 不换门赢);

    std::cout << "换门赢: " << 换门赢 << " (" << (static_cast<double>(换门赢) / 模拟次数 * 100) << "%)\n";
    std::cout << "不换门赢: " << 不换门赢 << " (" << (static_cast<double>(不换门赢) / 模拟次数 * 100) << "%)\n";
}