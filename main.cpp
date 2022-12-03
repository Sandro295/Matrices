#include <iostream>
#include <vector>
#include <random>
#include "matrix.h"

using Tensor = std::vector<Matrix>;

Matrix multiplyViaTensor(Matrix& a, Matrix& b, Tensor& tensor3d) {
    Matrix result(a.Rows(), b.Columns());
    for (int m = 0; m < tensor3d.size(); ++m) {
        for (int i = 0; i < tensor3d[0].Rows(); ++i) {
            for (int j = 0; j < tensor3d[0].Columns(); ++j) {
                if (tensor3d[m](i, j)) {
                    result(i, j) = a(i, j) * b(i, j);
                }
            }
        }
    }
    return result;
}


// u, v, w must have the same size

// indexing is (row, column)
Matrix multiplyDecomposed(const Matrix& a, const Matrix& b, const Matrix& u, const Matrix& v, const Matrix& w) {
    Matrix c(a.Rows(), b.Columns());
    std::vector<int> m(u.Columns());

    for (int i = 0; i < u.Columns(); ++i) {
        int a_sum = 0;
        int b_sum = 0;
        for (int j = 0; j < u.Rows(); ++j) {
            a_sum += a(j, i) * u(j, i);
            b_sum += b(j, i) * v(j, i);
        }
        m[i] = a_sum*b_sum;
    }

    std::vector<int> c_elements(a.Rows() * b.Columns())

    for (int i = 0; i < w.Columns(); ++i) {
        int m_sum = 0;
        for (int j = 0; j < w.Rows(); ++j) {
            m_sum += m[j] * w(j, i);
        }
        if (counter++ % a.Columns() != 0) {
        }
        c_elements[i] = m_sum;
    }

    for (int i = 0; i < a.Rows(); ++i) {
        for (int j = 0; j < b.Columns(); ++j) {
            c(j, i) = c_elements[b.Columns() * i + j];
        }
    }

    return c;
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

int main() {
    Matrix a(2);
    Matrix b(2);

    fillMatrixRandomly(a);
    fillMatrixRandomly(b);

    Matrix regularMult = a * b;

    printMatrix(regularMult);

    // Matrix alphatensored = multiplyDecomposed(a, b);

}
