#pragma once

#include "matrix.h"

bool EqualMatrices(const Matrix& a, const Matrix& b);
void printMatrix(Matrix& a);
void fillMatrixRandomly(Matrix& a);
void fillMatrixRandomly(double* matrix, size_t rows, size_t columns);
Matrix multiplyDecomposed(const Matrix& a, const Matrix& b, const Matrix& u, const Matrix& v, const Matrix& w);
Matrix multiplyBlocked(const Matrix& A, const Matrix& B, size_t blockSize);
