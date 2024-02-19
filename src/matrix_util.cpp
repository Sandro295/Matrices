#include "matrix_util.h"
#include <random>
#include <iostream>
#include "blis.h"
// #include <blis.hh>

static std::random_device rd;
static std::mt19937 generator{rd()};
static constexpr int range = 10;
static std::uniform_int_distribution<int> uid(-range, range);
static constexpr auto EPS = 1e-6;

bool EqualMatrices(const Matrix& a, const Matrix& b) {
    if (a.Rows() != b.Rows()) {
        return false;
    }
    if (a.Columns() != b.Columns()) {
        return false;
    }

    for (auto i = 0ul; i < a.Rows(); ++i) {
        for (auto j = 0ul; j < a.Columns(); ++j) {
            if (a(i, j) - b(i, j) > EPS) {
                return false;
            }
        }
    }
    return true;
}

bool EqualMatrices(const double* A, const double* B, size_t length) {
    for (size_t i = 0; i < length; ++i) {
        if (A[i] != B[i]) {
            return false;
        }
    }
    return true;
}

void printMatrix(Matrix& matrix) {
    for (size_t row = 0; row < matrix.Rows(); ++row) {
        std::cout << "\t";
        for (size_t col = 0; col < matrix.Columns(); ++col) {
            std::cout << matrix(row, col) << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void fillMatrixRandomly(Matrix& matrix) {
    fillMatrixRandomly(matrix.Data(), matrix.Rows(), matrix.Columns());
}

// contiguous fill
void fillMatrixRandomly(double* matrix, size_t rows, size_t columns) {
    for (size_t i = 0; i < (rows * columns); ++i) {
        matrix[i] = static_cast<double>(uid(generator));
    }
}

Matrix multiplyBlocked(const Matrix& A, const Matrix& B, size_t blockSize = 16) {
    Matrix new_mtx(A.Rows(), B.Columns());

    for (auto jj = 0u; jj < A.Rows(); jj += blockSize) {
        auto jjMin = std::min(jj + blockSize, A.Rows());
        for (auto kk = 0u; kk < A.Columns(); kk += blockSize) {
            auto kkMin = std::min(kk + blockSize, A.Columns());
            for (auto i = 0u; i < A.Rows(); i++) {
                for (auto j = jj; j < jjMin; j++) {
                    double acc = 0;
                    for (auto k = kk; k < kkMin; k++) {
                        acc += A(i, k) * B(k, j);
                    }
                    new_mtx(i, j) = acc;
                }
            }
        }
    }
    return new_mtx;
}

// indexing is (row, column)
// u, v, w must have the same size
Matrix multiplyDecomposed(const Matrix& a, const Matrix& b,
                          const Matrix& u, const Matrix& v, const Matrix& w) {
    Matrix c(a.Rows(), b.Columns());
    std::vector<int> m(u.Columns());

    for (size_t col = 0; col < u.Columns(); ++col) {
        int a_sum = 0;
        int b_sum = 0;
        for (size_t row = 0; row < u.Rows(); ++row) {
            a_sum += a(row) * u(row, col);
            b_sum += b(row) * v(row, col);
        }
        m[col] = a_sum*b_sum;
    }

    for (size_t row = 0; row < w.Rows(); ++row) {
        int m_sum = 0;
        for (size_t col = 0; col < w.Columns(); ++col) {
            m_sum += m[col] * w(row, col);
        }
        c(row) = m_sum;
    }
    return c;
}
