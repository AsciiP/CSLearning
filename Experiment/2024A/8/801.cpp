#include <iostream>

using namespace std;

struct Student
{
    int num;
    string name;
    int score[3];
};

void print(Student students[], int size)
{
    for (int i = 0; i < size; ++i)
    {
        cout << "Student " << i + 1 << ":" << endl;
        cout << "Num: " << students[i].num << endl;
        cout << "Name: " << students[i].name << endl;
        cout << "Scores: ";
        for (int j = 0; j < 3; ++j)
        {
            cout << students[i].score[j] << " ";
        }
        cout << endl;
    }
}

int main()
{
    Student students[5];
    for (int i = 0; i < 5; ++i)
    {
        cout << "Enter details for student " << i + 1 << ":" << endl;
        cout << "Num: ";
        cin >> students[i].num;
        cout << "Name: ";
        cin >> students[i].name;
        cout << "Enter 3 scores: ";
        for (int j = 0; j < 3; ++j)
        {
            cin >> students[i].score[j];
        }
    }

    print(students, 5);

    return 0;
}