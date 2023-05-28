#pragma once

#include "matrix.h"

bool EqualMatrices(const Matrix& a, const Matrix& b);
void printMatrix(Matrix& a);
void fillMatrixRandomly(Matrix& a);
void fillMatrixRandomly(double* matrix, int rows, int columns);
Matrix multiplyDecomposed(const Matrix& a, const Matrix& b, const Matrix& u, const Matrix& v, const Matrix& w);