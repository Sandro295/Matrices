#include <iostream>
#include <vector>
#include "matrix.h"
#include "matrix_util.h"
#include "Timer.h"

#include <mkl.h>

using Tensor = std::vector<Matrix>;

#define min(x,y) (((x) < (y)) ? (x) : (y))

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


int IntelExample() {
    double *A, *B, *C;
    int m, n, k, i, j;
    double alpha, beta;

    printf ("\n This example computes real matrix C=alpha*A*B+beta*C using \n"
            " Intel(R) MKL function dgemm, where A, B, and  C are matrices and \n"
            " alpha and beta are double precision scalars\n\n");

    m = 2000, k = 200, n = 1000;
    printf (" Initializing data for matrix multiplication C=A*B for matrix \n"
            " A(%ix%i) and matrix B(%ix%i)\n\n", m, k, k, n);
    alpha = 1.0; beta = 0.0;

    printf (" Allocating memory for matrices aligned on 64-byte boundary for better \n"
            " performance \n\n");
    A = (double *)mkl_malloc( m*k*sizeof( double ), 64 );
    B = (double *)mkl_malloc( k*n*sizeof( double ), 64 );
    C = (double *)mkl_malloc( m*n*sizeof( double ), 64 );
    if (A == NULL || B == NULL || C == NULL) {
        printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
        mkl_free(A);
        mkl_free(B);
        mkl_free(C);
        return 1;
    }

    printf (" Intializing matrix data \n\n");

    fillMatrixRandomly(A, m, k);
    fillMatrixRandomly(B, k, n);

    for (i = 0; i < (m*n); i++) {
        C[i] = 0.0;
    }
    
    printf (" Computing matrix product using Intel(R) MKL dgemm function via CBLAS interface \n\n");
{
    Timer t;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, n, k, alpha, A, k, B, n, beta, C, n);
}
    printf ("\n Computations completed.\n\n");

    printf (" Top left corner of matrix A: \n");
    for (i=0; i<min(m,6); i++) {
        for (j=0; j<min(k,6); j++) {
            printf ("%12.0f", A[j+i*k]);
        }
        printf ("\n");
    }

    printf ("\n Top left corner of matrix B: \n");
    for (i=0; i<min(k,6); i++) {
        for (j=0; j<min(n,6); j++) {
            printf ("%12.0f", B[j+i*n]);
        }
            printf ("\n");
    }
    
    printf ("\n Top left corner of matrix C: \n");
    for (i=0; i<min(m,6); i++) {
        for (j=0; j<min(n,6); j++) {
        printf ("%12.5G", C[j+i*n]);
        }
        printf ("\n");
    }

    printf ("\n Deallocating memory \n\n");
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    printf (" Example completed. \n\n");
    return 0;
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
    Timer t;
    IntelExample();
}
