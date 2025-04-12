#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    double operand1, operand2;
    char op;
    cin >> operand1 >> op >> operand2;

    cout << fixed << setprecision(2);

    switch (op) {
        case '+':
            cout << operand1 + operand2 << endl;
            break;
        case '-':
            cout << operand1 - operand2 << endl;
            break;
        case '*':
            cout << operand1 * operand2 << endl;
            break;
        case '/':
            if (operand2 == 0) {
                cout << "Divisor can not be 0!" << endl;
            } else {
                cout << operand1 / operand2 << endl;
            }
            break;
        default:
            cout << "Unknown operator!" << endl;
            break;
    }

    return 0;
}