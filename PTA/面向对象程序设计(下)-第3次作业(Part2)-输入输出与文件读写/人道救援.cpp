/*
  @pintia psid=1913051309887856640 pid=1913051309908828162 compiler=GXX
  ProblemSet: 面向对象程序设计(下)-第3次作业(Part2)-输入输出与文件读写
  Title: 人道救援
  https://pintia.cn/problem-sets/1913051309887856640/exam/problems/type/7?problemSetProblemId=1913051309908828162
*/
// @pintia code=start
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <cmath>
#include <iomanip>

using namespace std;

const double PI = acos(-1.0);
const double R = 6378.1;

struct City
{
    double lat, lng;
};

double deg2rad(double deg)
{
    return deg * PI / 180.0;
}

double calc_dist(const City &a, const City &b)
{
    double alat = deg2rad(a.lat);
    double blat = deg2rad(b.lat);
    double alng = deg2rad(a.lng);
    double blng = deg2rad(b.lng);
    double cos_theta = sin(alat) * sin(blat) + cos(alat) * cos(blat) * cos(alng - blng);
    // Clamp to [-1,1] to avoid NaN due to floating point error
    if (cos_theta > 1.0)
        cos_theta = 1.0;
    if (cos_theta < -1.0)
        cos_theta = -1.0;
    double theta = acos(cos_theta);
    return R * theta;
}

int main()
{
    map<string, City> city_map;
    string line;
    // Read header
    getline(cin, line);
    // Read city data
    while (getline(cin, line))
    {
        if (line == "=====")
            break;
        stringstream ss(line);
        string name, latstr, lngstr;
        getline(ss, name, ',');
        getline(ss, latstr, ',');
        getline(ss, lngstr, ',');
        City c;
        c.lat = stod(latstr);
        c.lng = stod(lngstr);
        city_map[name] = c;
    }
    // Read queries
    while (getline(cin, line))
    {
        if (line.empty())
            continue;
        stringstream ss(line);
        string city1, city2;
        getline(ss, city1, ',');
        getline(ss, city2);
        // Remove leading/trailing spaces
        city1.erase(0, city1.find_first_not_of(" \t\r\n"));
        city1.erase(city1.find_last_not_of(" \t\r\n") + 1);
        city2.erase(0, city2.find_first_not_of(" \t\r\n"));
        city2.erase(city2.find_last_not_of(" \t\r\n") + 1);
        double dist = calc_dist(city_map[city1], city_map[city2]);
        // For very small distances, output 0.0
        if (dist < 1e-6)
            dist = 0.0;
        cout << fixed << setprecision(1) << dist << endl;
    }
    return 0;
}
// @pintia code=end