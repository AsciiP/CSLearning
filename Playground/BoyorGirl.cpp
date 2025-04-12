#include <iostream>
#include <cstdlib>
#include <ctime>

int main() {
    const int 模拟次数 = 1000000;
    int 两个都是男孩 = 0;
    int 至少一个是男孩 = 0;

    std::srand(std::time(0));

    for (int i = 0; i < 模拟次数; ++i) {
        bool 第一个孩子是男孩 = std::rand() % 2;
        bool 第二个孩子是男孩 = std::rand() % 2;

        if (第一个孩子是男孩 || 第二个孩子是男孩) {
            至少一个是男孩++;
            if (第一个孩子是男孩 && 第二个孩子是男孩) {
                两个都是男孩++;
            }
        }
    }

    double 概率 = static_cast<double>(两个都是男孩) / 至少一个是男孩;
    std::cout << "在至少有一个是男孩的情况下，两个孩子都是男孩的概率: " << 概率 << std::endl;

    return 0;
}