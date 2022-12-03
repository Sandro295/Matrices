#include "matrix_util.h"
#include <random>
#include <iostream>

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

void printMatrix(Matrix& a) {
    for (int i = 0; i < a.Rows(); ++i) {
        for (int j = 0; j < a.Columns(); ++j) {
            std::cout << a(i, j) << "\t";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

void fillMatrixRandomly(Matrix& a) {
    static std::random_device rd;
    static std::mt19937 generator{rd()};
    static constexpr int range = 10;
    static std::uniform_int_distribution<int> uid(-range, range);

    for (int i = 0; i < a.Rows(); ++i) {
        for (int j = 0; j < a.Columns(); ++j) {
            a(i, j) = uid(generator);
        }
    }
}

// indexing is (row, column)
// u, v, w must have the same size
Matrix multiplyDecomposed(const Matrix& a, const Matrix& b, const Matrix& u, const Matrix& v, const Matrix& w) {
    Matrix c(a.Rows(), b.Columns());
    std::vector<int> m(u.Columns());

    for (int i = 0; i < u.Columns(); ++i) {
        int a_sum = 0;
        int b_sum = 0;
        for (int j = 0; j < u.Rows(); ++j) {
            a_sum += a(j) * u(j, i);
            b_sum += b(j) * v(j, i);
        }
        m[i] = a_sum*b_sum;
    }

    for (int i = 0; i < w.Rows(); ++i) {
        int m_sum = 0;
        for (int j = 0; j < w.Columns(); ++j) {
            m_sum += m[j] * w(i, j);
        }
        c(i) = m_sum;
    }

    return c;
}
