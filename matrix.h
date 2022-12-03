#pragma once

#include <vector>

class Matrix {
public:
    Matrix(int rows, int columns) {
        matrix_.resize(rows, std::vector<double>(columns));
    }

    Matrix(int square) : Matrix(square, square) {
    }

    Matrix(const std::vector<std::vector<double>> matrix) {
        matrix_.assign(matrix.begin(), matrix.end());
    }

    size_t Columns() const {
        return matrix_[0].size();
    }

    size_t Rows() const {
        return matrix_.size();
    }

    double& operator()(int row, int col) {
        return matrix_[row][col];
    }

    const double& operator()(int row, int col) const {
        return matrix_[row][col];
    }

    double& operator()(int index) {
        int row = index / Rows();
        int col = index % Columns();
        return matrix_[row][col];
    }

    const double& operator()(int index) const {
        int row = index / Rows();
        int col = index % Columns();
        return matrix_[row][col];
    }


    Matrix& operator*=(const Matrix& mtx) {
        *this* mtx;
        return *this;
    }

    Matrix& operator+=(const Matrix& mtx) {
        for (uint32_t i = 0; i < mtx.Rows(); ++i) {
            for (uint32_t j = 0; j < mtx.Columns(); ++j) {
                matrix_[i][j] += mtx(i, j);
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& mtx) {
        for (uint32_t i = 0; i < mtx.Rows(); ++i) {
            for (uint32_t j = 0; j < mtx.Columns(); ++j) {
                matrix_[i][j] -= mtx(i, j);
            }
        }
        return *this;
    }

    Matrix operator+(const Matrix& mtx) const {
        Matrix ret = *this;
        return ret += mtx;
    }

    Matrix operator-(const Matrix& mtx) const {
        Matrix ret = *this;
        return ret -= mtx;
    }

    Matrix operator*(const Matrix& mtx) const {
        Matrix new_mtx(Rows(), mtx.Columns());
        for (uint32_t j = 0; j < new_mtx.Columns(); ++j) {
            for (uint32_t i = 0; i < new_mtx.Rows(); ++i) {
                for (uint32_t k = 0; k < Columns(); ++k) {
                    new_mtx(i, j) += matrix_[i][k] * mtx(k, j);
                }
            }
        }
        return new_mtx;
    }

private:
    std::vector<std::vector<double>> matrix_;
};
