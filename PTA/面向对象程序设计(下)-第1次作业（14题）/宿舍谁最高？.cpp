/*
  @pintia psid=1903955893807046656 pid=1903955893832212487 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第1次作业（14题）
  Title: 宿舍谁最高？
  https://pintia.cn/problem-sets/1903955893807046656/exam/problems/type/7?problemSetProblemId=1903955893832212487
*/
// @pintia code=start
#include <iostream>
#include <string>
#include <map>
#include <iomanip>

struct Student {
    std::string name;
    int height;
    int weight;
};

int main() {
    int n;
    std::cin >> n;

    std::map<int, Student> dorms;

    for (int i = 0; i < n; ++i) {
        int dormNumber;
        std::string name;
        int height, weight;
        std::cin >> dormNumber >> name >> height >> weight;

        if (dorms.find(dormNumber) == dorms.end() || dorms[dormNumber].height < height) {
            dorms[dormNumber] = {name, height, weight};
        }
    }

    for (const auto &entry : dorms) {
        std::cout << std::setw(6) << std::setfill('0') << entry.first << " "
                  << entry.second.name << " "
                  << entry.second.height << " "
                  << entry.second.weight << std::endl;
    }

    return 0;
}
// @pintia code=end