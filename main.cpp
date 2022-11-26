#include <iostream>
#include "matrix.h"

Matrix multiplyViaTensor(Matrix& a, Matrix& b, Matrix& tensor) {
    Matrix result(a.Rows(), b.Columns());
    for (int i = 0; i < tensor.Rows(); ++i) {
        for (int j = 0; j < tensor.Columns(); ++j) {
            result(i, j) = a(i, j) * b(i, j);
        }
    }

    return result;
}

void printMatrix(Matrix& a) {
    for (int i = 0; i < a.Rows(); ++i) {
        for (int j = 0; j < a.Columns(); ++j) {
            std::cout << a(i, j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

int main() {
    Matrix a(5);
    Matrix b(5);

    Matrix regularMult = a * b;

    printMatrix(regularMult);

    Matrix tensored = multiplyViaTensor();

}