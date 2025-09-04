#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <fstream>

using namespace std;
using namespace std::chrono;
using Matrix = vector<vector<int>>;

// Utility: Create square matrix filled with a value
Matrix make_Matrix(int n, int val = 0) {
    return Matrix(n, vector<int>(n, val));
}

// Naive O(n³) multiplication
Matrix naiveMultiply(const Matrix &A, const Matrix &B) {
    int n = A.size();
    Matrix C(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            for (int k = 0; k < n; ++k)
                C[i][j] += A[i][k] * B[k][j];
    return C;
}

// Matrix addition
Matrix add(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C = make_Matrix(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = A[i][j] + B[i][j];
    return C;
}

// Matrix subtraction
Matrix sub(const Matrix& A, const Matrix& B) {
    int n = A.size();
    Matrix C = make_Matrix(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            C[i][j] = A[i][j] - B[i][j];
    return C;
}

// Split matrix into 4 quadrants
void split(const Matrix& M, Matrix& A11, Matrix& A12, Matrix& A21, Matrix& A22) {
    int n = M.size(), m = n / 2;
    A11 = make_Matrix(m);
    A12 = make_Matrix(m);
    A21 = make_Matrix(m);
    A22 = make_Matrix(m);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j) {
            A11[i][j] = M[i][j];
            A12[i][j] = M[i][j + m];
            A21[i][j] = M[i + m][j];
            A22[i][j] = M[i + m][j + m];
        }
}

// Join 4 quadrants into one matrix
Matrix join(const Matrix& C11, const Matrix& C12, const Matrix& C21, const Matrix& C22) {
    int m = C11.size(), n = m * 2;
    Matrix C = make_Matrix(n);
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < m; ++j) {
            C[i][j] = C11[i][j];
            C[i][j + m] = C12[i][j];
            C[i + m][j] = C21[i][j];
            C[i + m][j + m] = C22[i][j];
        }
    return C;
}

// Strassen’s recursive multiplication
Matrix strassenMultiply(const Matrix& A, const Matrix& B, int threshold = 64) {
    int n = A.size();
    if (n <= threshold) return naiveMultiply(A, B);

    Matrix A11, A12, A21, A22;
    Matrix B11, B12, B21, B22;
    split(A, A11, A12, A21, A22);
    split(B, B11, B12, B21, B22);

    Matrix M1 = strassenMultiply(add(A11, A22), add(B11, B22), threshold);
    Matrix M2 = strassenMultiply(add(A21, A22), B11, threshold);
    Matrix M3 = strassenMultiply(A11, sub(B12, B22), threshold);
    Matrix M4 = strassenMultiply(A22, sub(B21, B11), threshold);
    Matrix M5 = strassenMultiply(add(A11, A12), B22, threshold);
    Matrix M6 = strassenMultiply(sub(A21, A11), add(B11, B12), threshold);
    Matrix M7 = strassenMultiply(sub(A12, A22), add(B21, B22), threshold);

    Matrix C11 = add(sub(add(M1, M4), M5), M7);
    Matrix C12 = add(M3, M5);
    Matrix C21 = add(M2, M4);
    Matrix C22 = add(sub(add(M1, M3), M2), M6);

    return join(C11, C12, C21, C22);
}

// Pad matrix to next power of 2
int next_pow2(int n) {
    int p = 1;
    while (p < n) p <<= 1;
    return p;
}

Matrix pad(const Matrix& A, int sz) {
    int n = A.size();
    Matrix P = make_Matrix(sz);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            P[i][j] = A[i][j];
    return P;
}

Matrix unpad(const Matrix& A, int n) {
    Matrix U = make_Matrix(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            U[i][j] = A[i][j];
    return U;
}

// Wrapper to handle padding
Matrix strassen_wrapper(const Matrix& A, const Matrix& B, int threshold = 64) {
    int n = A.size();
    int m = next_pow2(n);
    if (m == n) return strassenMultiply(A, B, threshold);
    Matrix Ap = pad(A, m);
    Matrix Bp = pad(B, m);
    Matrix Cp = strassenMultiply(Ap, Bp, threshold);
    return unpad(Cp, n);
}

// Main benchmarking logic
int main() {
    vector<double> naiveTimes, strassenTimes;
    ofstream file("Task_1_timings.csv");
    if (!file.is_open()) {
        cerr << "Error: Could not open file for writing.\n";
        return 1;
    }
    file << "Size,Naive Time (ms),Strassen Time (ms)\n";

    int size = 4;
    for (int i = 0; i < 8; i++) {
        size *= 2;
        Matrix A(size, vector<int>(size));
        Matrix B(size, vector<int>(size));
        for (int i = 0; i < size; ++i)
            for (int j = 0; j < size; ++j) {
                A[i][j] = rand() % 1000;
                B[i][j] = rand() % 1000;
            }

        auto start = high_resolution_clock::now();
        Matrix C1 = naiveMultiply(A, B);
        auto end = high_resolution_clock::now();
        double naiveTime = duration<double, milli>(end - start).count();
        naiveTimes.push_back(naiveTime);

        start = high_resolution_clock::now();
        Matrix C2 = strassen_wrapper(A, B);
        end = high_resolution_clock::now();
        double strassenTime = duration<double, milli>(end - start).count();
        strassenTimes.push_back(strassenTime);

        cout << "\033[1;34mSize: " << size << "\033[0m | "
             << "Naive: " << naiveTime << " ms | "
             << "Strassen: " << strassenTime << " ms\n";

        file << size << "," << naiveTime << "," << strassenTime << "\n";
    }

    file.close();
    return 0;
}