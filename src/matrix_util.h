#pragma once

#include "matrix.h"

bool EqualMatricies(const Matrix& a, const Matrix& b);
void printMatrix(Matrix& a);
void fillMatrixRandomly(Matrix& a);
Matrix multiplyDecomposed(const Matrix& a, const Matrix& b, const Matrix& u, const Matrix& v, const Matrix& w);
