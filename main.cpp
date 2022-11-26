#include <iostream>
#include <vector>
#include <random>
#include "matrix.h"

Matrix multiplyViaTensor(Matrix& a, Matrix& b, std::vector<Matrix>& tensor3d) {
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
    Matrix a(5);
    Matrix b(5);

    fillMatrixRandomly(a);
    fillMatrixRandomly(b);

    Matrix regularMult = a * b;

    printMatrix(regularMult);

    // Matrix tensored = multiplyViaTensor();

}
