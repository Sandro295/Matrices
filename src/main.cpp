#include <iostream>
#include <vector>
#include "matrix_util.h"
#include "Timer.h"

#include "mkl_util.h"

using Tensor = std::vector<Matrix>;

Matrix multiplyViaTensor(Matrix& a, Matrix& b, Tensor& tensor3d) {
    Matrix result(a.Rows(), b.Columns());
    for (size_t m = 0; m < tensor3d.size(); ++m) {
        for (size_t i = 0; i < tensor3d[0].Rows(); ++i) {
            for (size_t j = 0; j < tensor3d[0].Columns(); ++j) {
                if (tensor3d[m](i, j)) {
                    result(i, j) = a(i, j) * b(i, j);
                }
            }
        }
    }
    return result;
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

void benchMyMultiply(uint32_t times, int a_rows, int a_cols_b_rows, int b_cols) {
    Timer t(__FUNCTION__);
    while(times--) {
        Matrix a(a_rows, a_cols_b_rows);
        Matrix b(a_cols_b_rows, b_cols);

        fillMatrixRandomly(a);
        fillMatrixRandomly(b);

        Matrix regularMult = a * b;
    }
}

void benchMKL(uint32_t times, int a_rows, int a_cols_b_rows, int b_cols) {
    Timer t(__FUNCTION__);
    while(times--) {
        IntelExample(a_rows, a_cols_b_rows, b_cols);
    }
}

int main() {
    constexpr uint32_t timesToRepeat = 1;
    constexpr int a_rows = 1399;
    constexpr int a_cols_b_rows = 2439;
    constexpr int b_rows = 4101;
    benchMKL(timesToRepeat, a_rows, a_cols_b_rows, b_rows);
    benchMyMultiply(timesToRepeat, a_rows, a_cols_b_rows, b_rows);
}
