#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int n;
long long count_brute = 0; // Number of solutions found by brute force
long long count_backtrack = 0; // Number of solutions found by backtracking

// ================= Method 1: Pure Brute Force Recursion (No Pruning) =================
// Only ensures different rows and columns; checks diagonals at the very end
bool isValid(vector<int>& board) {
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (abs(i - j) == abs(board[i] - board[j])) {
                return false;
            }
        }
    }
    return true;
}

void solveBruteForce(vector<int>& board, int row) {
    if (row == n) {
        if (isValid(board)) {
            count_brute++;
        }
        return;
    }
    for (int col = 0; col < n; ++col) {
        board[row] = col;
        solveBruteForce(board, row + 1); // Recurse to the last row regardless
    }
}

// ================= Method 2: Backtracking (With Pruning) =================
// Checks diagonals immediately when placing; prunes if there is a conflict
bool isSafe(vector<int>& board, int row, int col) {
    for (int i = 0; i < row; ++i) {
        // Check column conflict (actually guaranteed by the loop, mainly checking diagonals here)
        // Check main diagonal and anti-diagonal
        if (abs(row - i) == abs(col - board[i])) {
            return false;
        }
    }
    return true;
}

void solveBacktracking(vector<int>& board, int row) {
    if (row == n) {
        count_backtrack++;
        return;
    }
    for (int col = 0; col < n; ++col) {
        if (isSafe(board, row, col)) { // Pruning: if not safe, do not enter the next level
            board[row] = col;
            solveBacktracking(board, row + 1);
        }
    }
}

// ================= Performance Test Main Function =================
int main() {
    // For demonstration, we test N=12 (Brute force becomes very slow when N>13)
    int test_n = 8; 
    
    cout << "Testing N-Queens problem for N = " << test_n << "..." << endl;

    // 1. Test Brute Force Recursion
    n = test_n;
    vector<int> board1(n, 0);
    auto start1 = chrono::high_resolution_clock::now();
    solveBruteForce(board1, 0);
    auto end1 = chrono::high_resolution_clock::now();
    double time1 = chrono::duration<double, milli>(end1 - start1).count();

    // 2. Test Backtracking
    vector<int> board2(n, 0);
    auto start2 = chrono::high_resolution_clock::now();
    solveBacktracking(board2, 0);
    auto end2 = chrono::high_resolution_clock::now();
    double time2 = chrono::duration<double, milli>(end2 - start2).count();

    // Output results
    cout << "----------------------------------------" << endl;
    cout << "Method 1: Pure Brute Force Recursion (No Pruning)" << endl;
    cout << "Number of solutions found: " << count_brute << endl;
    cout << "Time elapsed: " << time1 << " ms" << endl;

    cout << "----------------------------------------" << endl;
    cout << "Method 2: Backtracking (With Pruning)" << endl;
    cout << "Number of solutions found: " << count_backtrack << endl;
    cout << "Time elapsed: " << time2 << " ms" << endl;
    
    cout << "----------------------------------------" << endl;
    cout << "Performance improvement: " << time1 / time2 << " times" << endl;

    return 0;
}