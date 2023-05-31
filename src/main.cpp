#include <iostream>
#include <vector>
#include "matrix_util.h"
#include "Timer.h"

#include "mkl_util.h"
#include "aocl_util.h"

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

static const Matrix u( \
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

void benchMyMultiply(int times, int a_rows, int a_cols_b_rows, int b_cols) {
    Timer t(__FUNCTION__);
    // std::vector<Matrix> A_matrices{times};
    // std::vector<Matrix> B_matrices{times};
    // std::vector<Matrix> C_matrices{times};
    // for (auto &matrix : A_matrices) {
    //     fillMatrixRandomly(matrix);
    // }
    // for (auto &matrix : B_matrices) {
    //     fillMatrixRandomly(matrix);
    // }

    // Timer t(__FUNCTION__);
    // for (auto i = 0; i < C_matrices.size(); ++i) {
    //     C_matrices[i] = A_matrices[i] * B_matrices[i];
    // }

    Matrix a(a_rows, a_cols_b_rows);
    Matrix b(a_cols_b_rows, b_cols);
    Matrix regularMult;
    while(times--) {
        fillMatrixRandomly(a);
        fillMatrixRandomly(b);
        regularMult = a * b;
    }
}

void benchAlphaTensored(uint32_t times, int a_rows=2, int a_cols_b_rows=2, int b_cols=2) {
    Timer t(__FUNCTION__);
    Matrix a(a_rows, a_cols_b_rows);
    Matrix b(a_cols_b_rows, b_cols);
    Matrix alphatensored;
    while(times--) {
        fillMatrixRandomly(a);
        fillMatrixRandomly(b);
        alphatensored = multiplyDecomposed(a, b, u, v, w);
    }
}

void benchMKL(uint32_t times, int a_rows, int a_cols_b_rows, int b_cols) {
    Timer t(__FUNCTION__);
    while(times--) {
        IntelCBLASExample(a_rows, a_cols_b_rows, b_cols);
    }
}

void benchAOCL(uint32_t times, int a_rows, int a_cols_b_rows, int b_cols) {
    Timer t(__FUNCTION__);
    while(times--) {
        AmdBLISExample(a_rows, a_cols_b_rows, b_cols);
    }
}


int main() {
    constexpr uint32_t timesToRepeat = 1'000;
    constexpr int a_rows = 77;
    constexpr int a_cols_b_rows = 55;
    constexpr int b_rows = 99;
    benchMKL(timesToRepeat, a_rows, a_cols_b_rows, b_rows);
    benchAOCL(timesToRepeat, a_rows, a_cols_b_rows, b_rows);
    benchMyMultiply(timesToRepeat, a_rows, a_cols_b_rows, b_rows);
    benchAlphaTensored(timesToRepeat);
    return 0;
}
