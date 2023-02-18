#include "matrix_util.h"
#include <random>
#include <iostream>

static std::random_device rd;
static std::mt19937 generator{rd()};
static constexpr int range = 10;
static std::uniform_int_distribution<int> uid(-range, range);

bool EqualMatricies(const Matrix& a, const Matrix& b) {
    if (a.Rows() != b.Rows()) {
        return false;
    }
    if (a.Columns() != b.Columns()) {
        return false;
    }
    for (auto i = 0ul; i < a.Rows(); ++i) {
        for (auto j = 0ul; j < a.Columns(); ++j) {
            if (a(i, j) != b(i, j)) { // double could have some rounding error
                return false;
            }
        }
    }
    return true;
}

void printMatrix(Matrix& matrix) {
    for (size_t i = 0; i < matrix.Rows(); ++i) {
        std::cout << "\t";
        for (size_t j = 0; j < matrix.Columns(); ++j) {
            std::cout << matrix(i, j) << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void fillMatrixRandomly(Matrix& matrix) {
    for (size_t i = 0; i < matrix.Rows(); ++i) {
        for (size_t j = 0; j < matrix.Columns(); ++j) {
            matrix(i, j) = uid(generator);
        }
    }
}

// contiguous fill
void fillMatrixRandomly(double* matrix, int rows, int columns) {
    for (size_t i = 0; i < (rows * columns); ++i) {
        matrix[i] = static_cast<double>(uid(generator));
    }
}

// indexing is (row, column)
// u, v, w must have the same size
Matrix multiplyDecomposed(const Matrix& a, const Matrix& b, const Matrix& u, const Matrix& v, const Matrix& w) {
    Matrix c(a.Rows(), b.Columns());
    std::vector<int> m(u.Columns());

    for (size_t i = 0; i < u.Columns(); ++i) {
        int a_sum = 0;
        int b_sum = 0;
        for (size_t j = 0; j < u.Rows(); ++j) {
            a_sum += a(j) * u(j, i);
            b_sum += b(j) * v(j, i);
        }
        m[i] = a_sum*b_sum;
    }

    for (size_t i = 0; i < w.Rows(); ++i) {
        int m_sum = 0;
        for (size_t j = 0; j < w.Columns(); ++j) {
            m_sum += m[j] * w(i, j);
        }
        c(i) = m_sum;
    }
    return c;
}
