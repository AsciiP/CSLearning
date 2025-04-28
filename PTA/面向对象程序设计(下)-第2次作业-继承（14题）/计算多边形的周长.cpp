/*
  @pintia psid=1910112809178632192 pid=1910112809203798025 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第2次作业-继承（14题）
  Title: 计算多边形的周长
  https://pintia.cn/problem-sets/1910112809178632192/exam/problems/type/7?problemSetProblemId=1910112809203798025
*/
// @pintia code=start
#include <iostream>
using namespace std;

class polygon
{
protected:
    int number;
    int side_length[100];
public:
    polygon() : number(0)
    {
        for (int i = 0; i < 100; i++)
        {
            side_length[i] = 0;
        }
    }

    void set_sides(int sides[], int count)
    {
        number = count;
        for (int i = 0; i < count; i++)
        {
            side_length[i] = sides[i];
        }
    }

    int perimeter()
    {
        int sum = 0;
        for (int i = 0; i < number; i++)
        {
            sum += side_length[i];
        }
        return sum;
    }

    void display()
    {
        cout << number << " " << perimeter() << endl;
    }
};

class rectangle : public polygon
{
private:
    int height, width;

public:
    rectangle() : height(0), width(0) {}

    rectangle(int h, int w) : height(h), width(w)
    {
        number = 4;
        side_length[0] = height;
        side_length[1] = width;
        side_length[2] = height;
        side_length[3] = width;
    }

    void set_dimensions(int h, int w)
    {
        height = h;
        width = w;
        number = 4;
        side_length[0] = height;
        side_length[1] = width;
        side_length[2] = height;
        side_length[3] = width;
    }

    int perimeter()
    {
        return 2 * (height + width);
    }

    void display()
    {
        cout << number << " " << perimeter() << endl;
    }
};

class equal_polygon : public polygon
{
private:
    int side_len;

public:
    equal_polygon() : side_len(0) {}

    equal_polygon(int sides, int len) : side_len(len)
    {
        number = sides;
        for (int i = 0; i < number; i++)
        {
            side_length[i] = side_len;
        }
    }

    void set_equal_sides(int sides, int len)
    {
        number = sides;
        side_len = len;
        for (int i = 0; i < number; i++)
        {
            side_length[i] = side_len;
        }
    }

    int perimeter()
    {
        return number * side_len;
    }

    void display()
    {
        cout << number << " " << perimeter() << endl;
    }
};

int main() {
    int n;
    cin >> n;

    for (int i = 0; i < n; i++) {
        int type;
        cin >> type;

        if (type == 0) {
            polygon p;
            int sides[100];
            int count = 0;
            int len;
            while (cin >> len && len != -1) {
                sides[count++] = len;
            }
            p.set_sides(sides, count);
            p.display();
        }
        else if (type == 1) {
            rectangle r;
            int h, w;
            cin >> h >> w;
            r.set_dimensions(h, w);
            r.display();
        }
        else if (type == 2) {
            equal_polygon ep;
            int sides, len;
            cin >> sides >> len;
            ep.set_equal_sides(sides, len);
            ep.display();
        }
    }

    return 0;
}
// @pintia code=end