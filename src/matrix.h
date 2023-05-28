#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>

class Matrix {
public:
    Matrix(size_t rows, size_t columns) : rows_{rows}, columns_{columns} {
        matrix_.resize(rows_ * columns_);
    }

    Matrix(size_t square) : Matrix(square, square) {
    }

    Matrix(const std::vector<std::vector<double>> matrix) {
        size_t index = 0;
        rows_ = matrix.size();
        columns_ = matrix[0].size();
        matrix_.resize(rows_ * columns_);
        for (auto& row : matrix) {
            for (auto el : row) {
                matrix_[index++] = el;
            }
        }
    }

    size_t Columns() const {
        return columns_;
    }

    size_t Rows() const {
        return rows_;
    }

    double& operator()(size_t row, size_t col) {
        return matrix_[row * columns_ + col];
    }

    const double& operator()(size_t row, size_t col) const {
        return matrix_[row * columns_ + col];
    }

    double& operator()(size_t index) {
        return matrix_[index];
    }

    const double& operator()(size_t index) const {
        return matrix_[index];
    }


    Matrix& operator*=(const Matrix& mtx) {
        *this* mtx;
        return *this;
    }

    Matrix& operator+=(const Matrix& mtx) {
        for (uint32_t i = 0; i < mtx.Rows(); ++i) {
            for (uint32_t j = 0; j < mtx.Columns(); ++j) {
                matrix_[i * columns_ + j] += mtx(i, j);
            }
        }
        return *this;
    }

    Matrix& operator-=(const Matrix& mtx) {
        for (uint32_t i = 0; i < mtx.Rows(); ++i) {
            for (uint32_t j = 0; j < mtx.Columns(); ++j) {
                matrix_[i * columns_ + j] -= mtx(i, j);
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
        for (uint32_t col = 0; col < new_mtx.Columns(); ++col) {
            for (uint32_t row = 0; row < new_mtx.Rows(); ++row) {
                for (uint32_t k = 0; k < Columns(); ++k) {
                    new_mtx(row, col) += matrix_[row * columns_ + k] * mtx(k, col);
                }
            }
        }
        return new_mtx;
    }

private:
    size_t rows_;
    size_t columns_;
    std::vector<double> matrix_;
};
