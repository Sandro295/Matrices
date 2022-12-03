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

// [[[1, 0, 0, 1], [1, 0, 0, 1], [1, 0, 0, 1]],
// [[1, 0, 0, 0], [0, 1, 0, -1], [0, 0, 1, 1]],
// [[0, 1, 0, -1], [0, 0, 1, 1], [1, 0, 0, 0]],
// [[0, 0, 1, 1], [1, 0, 0, 0], [0, 1, 0, -1]],
// [[0, 0, 0, 1], [-1, 0, 1, 0], [1, 1, 0, 0]],
// [[-1, 0, 1, 0], [1, 1, 0, 0], [0, 0, 0, 1]],
// [[1, 1, 0, 0], [0, 0, 0, 1], [-1, 0, 1, 0]]]

// using std::vector<std::vector<int>> = matrix;

static const Matrix u ( \
{
    {1, 0, 1, 0, 1, -1, 0},
    {0, 0, 0, 0, 1, 0, 1},
    {0, 1, 0, 0, 0, 1, 0},
    {1, 1, 0, 1, 0, 0, -1}
});

static const Matrix v( \
{
    {1, 1, 0, -1, 0, 1, 0},
    {0, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 1, 0, 0, 1},
    {1, 0, -1, 0, 1, 0, 1}
});

static const Matrix w( \
{
    {1, 0, 0, 1, -1, 0, 1},
    {0, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 1, 0, 0, 0},
    {1, -1, 1, 0, 0, 1, 0}
});


// u, v, w must have the same size

// indexing is (row, column)
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

    std::vector<int> c_elements(a.Rows() * b.Columns());

    for (int i = 0; i < w.Rows(); ++i) {
        int m_sum = 0;
        for (int j = 0; j < w.Columns(); ++j) {
            m_sum += m[j] * w(i, j);
        }
        c(i) = m_sum;
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

    Matrix alphatensored = multiplyDecomposed(a, b, u, v, w);

    printMatrix(alphatensored);

    if (EqualMatricies(regularMult, alphatensored)) {
        std::cout << "\nSuccess!!!\n";
    }

}
