#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <cmath>
#include <iomanip>

using Matrix = std::vector<double>;

// 创建 n×n 零矩阵
Matrix create_matrix(int n) {
    return Matrix(n * n, 0.0);
}

// 获取元素 (i, j)
inline double& get(Matrix& M, int n, int i, int j) {
    return M[i * n + j];
}
inline const double& get(const Matrix& M, int n, int i, int j) {
    return M[i * n + j];
}

// 朴素三重循环 (i-k-j 顺序，缓存友好)
void naive_multiply(const Matrix& A, const Matrix& B, Matrix& C, int n, int threshold = 0) {
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n; ++k) {
            double a_ik = get(A, n, i, k);
            for (int j = 0; j < n; ++j) {
                get(C, n, i, j) += a_ik * get(B, n, k, j);
            }
        }
    }
}

// 矩阵加法 C = A + B，大小为 n×n
void mat_add(const Matrix& A, const Matrix& B, Matrix& C, int n) {
    for (int i = 0; i < n * n; ++i) {
        C[i] = A[i] + B[i];
    }
}

// 矩阵减法 C = A - B
void mat_sub(const Matrix& A, const Matrix& B, Matrix& C, int n) {
    for (int i = 0; i < n * n; ++i) {
        C[i] = A[i] - B[i];
    }
}

// 朴素分治乘法（带阈值）
void naive_divide_multiply(const Matrix& A, const Matrix& B, Matrix& C, int n, int threshold) {
    if (n <= threshold) {
        naive_multiply(A, B, C, n);
        return;
    }
    int mid = n / 2;
    int sub_n = mid;
    int sub_size = sub_n * sub_n;

    // 子矩阵视图：使用偏移量，避免拷贝
    auto sub = [&](const Matrix& M, int row_off, int col_off) -> const Matrix& {
        return M; // 通过偏移访问，实际在递归中用指针偏移
    };

    // 为简洁，这里直接分配子矩阵（实际优化可用指针偏移）
    Matrix A11(sub_size), A12(sub_size), A21(sub_size), A22(sub_size);
    Matrix B11(sub_size), B12(sub_size), B21(sub_size), B22(sub_size);
    Matrix C11(sub_size), C12(sub_size), C21(sub_size), C22(sub_size);

    // 拆分 A 和 B
    for (int i = 0; i < mid; ++i) {
        for (int j = 0; j < mid; ++j) {
            A11[i * mid + j] = get(A, n, i, j);
            A12[i * mid + j] = get(A, n, i, j + mid);
            A21[i * mid + j] = get(A, n, i + mid, j);
            A22[i * mid + j] = get(A, n, i + mid, j + mid);

            B11[i * mid + j] = get(B, n, i, j);
            B12[i * mid + j] = get(B, n, i, j + mid);
            B21[i * mid + j] = get(B, n, i + mid, j);
            B22[i * mid + j] = get(B, n, i + mid, j + mid);
        }
    }

    // 递归计算 8 次乘法
    Matrix T1(sub_size), T2(sub_size), T3(sub_size), T4(sub_size);

    naive_divide_multiply(A11, B11, T1, mid, threshold);
    naive_divide_multiply(A12, B21, T2, mid, threshold);
    mat_add(T1, T2, C11, mid);

    naive_divide_multiply(A11, B12, T1, mid, threshold);
    naive_divide_multiply(A12, B22, T2, mid, threshold);
    mat_add(T1, T2, C12, mid);

    naive_divide_multiply(A21, B11, T1, mid, threshold);
    naive_divide_multiply(A22, B21, T2, mid, threshold);
    mat_add(T1, T2, C21, mid);

    naive_divide_multiply(A21, B12, T1, mid, threshold);
    naive_divide_multiply(A22, B22, T2, mid, threshold);
    mat_add(T1, T2, C22, mid);

    // 合并结果到 C
    for (int i = 0; i < mid; ++i) {
        for (int j = 0; j < mid; ++j) {
            get(C, n, i, j) = C11[i * mid + j];
            get(C, n, i, j + mid) = C12[i * mid + j];
            get(C, n, i + mid, j) = C21[i * mid + j];
            get(C, n, i + mid, j + mid) = C22[i * mid + j];
        }
    }
}

// Strassen 乘法（带阈值）
void strassen_multiply(const Matrix& A, const Matrix& B, Matrix& C, int n, int threshold) {
    if (n <= threshold) {
        naive_multiply(A, B, C, n);
        return;
    }
    int mid = n / 2;
    int sub_n = mid;
    int sub_size = sub_n * sub_n;

    Matrix A11(sub_size), A12(sub_size), A21(sub_size), A22(sub_size);
    Matrix B11(sub_size), B12(sub_size), B21(sub_size), B22(sub_size);
    Matrix M1(sub_size), M2(sub_size), M3(sub_size), M4(sub_size);
    Matrix M5(sub_size), M6(sub_size), M7(sub_size);
    Matrix T1(sub_size), T2(sub_size), T3(sub_size), T4(sub_size);

    // 拆分
    for (int i = 0; i < mid; ++i) {
        for (int j = 0; j < mid; ++j) {
            A11[i * mid + j] = get(A, n, i, j);
            A12[i * mid + j] = get(A, n, i, j + mid);
            A21[i * mid + j] = get(A, n, i + mid, j);
            A22[i * mid + j] = get(A, n, i + mid, j + mid);

            B11[i * mid + j] = get(B, n, i, j);
            B12[i * mid + j] = get(B, n, i, j + mid);
            B21[i * mid + j] = get(B, n, i + mid, j);
            B22[i * mid + j] = get(B, n, i + mid, j + mid);
        }
    }

    // 计算 7 个 M
    mat_add(A11, A22, T1, mid);
    mat_add(B11, B22, T2, mid);
    strassen_multiply(T1, T2, M1, mid, threshold);

    mat_add(A21, A22, T1, mid);
    strassen_multiply(T1, B11, M2, mid, threshold);

    mat_sub(B12, B22, T1, mid);
    strassen_multiply(A11, T1, M3, mid, threshold);

    mat_sub(B21, B11, T1, mid);
    strassen_multiply(A22, T1, M4, mid, threshold);

    mat_add(A11, A12, T1, mid);
    strassen_multiply(T1, B22, M5, mid, threshold);

    mat_sub(A21, A11, T1, mid);
    mat_add(B11, B12, T2, mid);
    strassen_multiply(T1, T2, M6, mid, threshold);

    mat_sub(A12, A22, T1, mid);
    mat_add(B21, B22, T2, mid);
    strassen_multiply(T1, T2, M7, mid, threshold);

    // 组合 C
    Matrix C11(sub_size), C12(sub_size), C21(sub_size), C22(sub_size);

    // C11 = M1 + M4 - M5 + M7
    mat_add(M1, M4, T1, mid);
    mat_sub(T1, M5, T2, mid);
    mat_add(T2, M7, C11, mid);

    // C12 = M3 + M5
    mat_add(M3, M5, C12, mid);

    // C21 = M2 + M4
    mat_add(M2, M4, C21, mid);

    // C22 = M1 - M2 + M3 + M6  （修正：原为 M1-M2-M3+M6，应为 M1-M2+M3+M6）
    mat_sub(M1, M2, T1, mid);
    mat_add(T1, M3, T2, mid);
    mat_add(T2, M6, C22, mid);

    // 合并
    for (int i = 0; i < mid; ++i) {
        for (int j = 0; j < mid; ++j) {
            get(C, n, i, j) = C11[i * mid + j];
            get(C, n, i, j + mid) = C12[i * mid + j];
            get(C, n, i + mid, j) = C21[i * mid + j];
            get(C, n, i + mid, j + mid) = C22[i * mid + j];
        }
    }
}

// 基准测试
double benchmark(void (*func)(const Matrix&, const Matrix&, Matrix&, int, int),
                 const Matrix& A, const Matrix& B, Matrix& C, int n, int threshold) {
    Matrix C_work = C;
    std::fill(C_work.begin(), C_work.end(), 0.0);

    auto t0 = std::chrono::high_resolution_clock::now();
    func(A, B, C_work, n, threshold);
    auto t1 = std::chrono::high_resolution_clock::now();

    C = C_work;
    return std::chrono::duration<double>(t1 - t0).count();
}

// 相对误差（Frobenius 范数）
double relative_error(const Matrix& C1, const Matrix& C2, int n) {
    double diff = 0.0, norm = 0.0;
    for (int i = 0; i < n * n; ++i) {
        double d = C1[i] - C2[i];
        diff += d * d;
        norm += C2[i] * C2[i];
    }
    return std::sqrt(diff) / std::sqrt(norm);
}

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    const int threshold = 64;
    std::vector<int> sizes = {128, 256, 512, 1024};

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "n     | naive(s) | divide(s) | strassen(s) | speedup | error\n";
    std::cout << "------|----------|-----------|-------------|---------|----------\n";

    for (int n : sizes) {
        Matrix A(n * n), B(n * n), C_naive(n * n), C_divide(n * n), C_strassen(n * n);

        // 生成随机矩阵
        for (int i = 0; i < n * n; ++i) {
            A[i] = dist(gen);
            B[i] = dist(gen);
        }

        double t_naive = benchmark(naive_multiply, A, B, C_naive, n, threshold);
        double t_divide = benchmark(naive_divide_multiply, A, B, C_divide, n, threshold);
        double t_strassen = benchmark(strassen_multiply, A, B, C_strassen, n, threshold);

        double speedup = t_naive / t_strassen;
        double err = relative_error(C_naive, C_strassen, n);

        std::cout << std::setw(4) << n << "  | "
                  << std::setw(8) << t_naive << " | "
                  << std::setw(9) << t_divide << " | "
                  << std::setw(11) << t_strassen << " | "
                  << std::setw(7) << std::fixed << std::setprecision(2) << speedup << "x | "
                  << std::scientific << std::setprecision(1) << err << "\n";
    }

    return 0;
}