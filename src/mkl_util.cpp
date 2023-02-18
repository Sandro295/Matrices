#include "mkl_util.h"
#include "matrix_util.h"
#include <iostream>
#include "Timer.h"

#define min(x,y) (((x) < (y)) ? (x) : (y))

int IntelExample(int a_rows, int a_cols_b_rows, int b_cols) {
    double *A, *B, *C;
    int i, j;
    double alpha, beta;
#ifdef DBG
    printf ("\n This example computes real matrix C=alpha*A*B+beta*C using \n"
            " Intel(R) MKL function dgemm, where A, B, and  C are matrices and \n"
            " alpha and beta are double precision scalars\n\n");

    // a_rows = 2000, a_cols_b_rows = 200, b_cols = 1000;
    printf (" Initializing data for matrix multiplication C=A*B for matrix \n"
            " A(%ix%i) and matrix B(%ix%i)\n\n", a_rows, a_cols_b_rows, a_cols_b_rows, b_cols);
#endif
    alpha = 1.0; beta = 0.0;
#ifdef DBG
    printf (" Allocating memory for matrices aligned on 64-byte boundary for better \n"
            " performance \n\n");
#endif
    A = (double *)mkl_malloc( a_rows*a_cols_b_rows*sizeof( double ), 64 );
    B = (double *)mkl_malloc( a_cols_b_rows*b_cols*sizeof( double ), 64 );
    C = (double *)mkl_malloc( a_rows*b_cols*sizeof( double ), 64 );
    if (A == NULL || B == NULL || C == NULL) {
        printf( "\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
        mkl_free(A);
        mkl_free(B);
        mkl_free(C);
        return 1;
    }

    fillMatrixRandomly(A, a_rows, a_cols_b_rows);
    fillMatrixRandomly(B, a_cols_b_rows, b_cols);

    for (i = 0; i < (a_rows*b_cols); i++) {
        C[i] = 0.0;
    }

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                a_rows, b_cols, a_cols_b_rows, alpha, A, a_cols_b_rows, B, b_cols, beta, C, b_cols);

#ifdef DBG
    printf (" Top left corner of matrix A: \n");
    for (i=0; i<min(a_rows,6); i++) {
        for (j=0; j<min(a_cols_b_rows,6); j++) {
            printf ("%12.0f", A[j+i*a_cols_b_rows]);
        }
        printf ("\n");
    }

    printf ("\n Top left corner of matrix B: \n");
    for (i=0; i<min(a_cols_b_rows,6); i++) {
        for (j=0; j<min(b_cols,6); j++) {
            printf ("%12.0f", B[j+i*b_cols]);
        }
            printf ("\n");
    }
    
    printf ("\n Top left corner of matrix C: \n");
    for (i=0; i<min(a_rows,6); i++) {
        for (j=0; j<min(b_cols,6); j++) {
        printf ("%12.5G", C[j+i*b_cols]);
        }
        printf ("\n");
    }
#endif
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);

    return 0;
}
