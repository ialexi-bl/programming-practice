#pragma once
#include "math.hpp"
#include <iostream>
#include <utility>
#include <vector>

namespace math
{
    using matrix_value = value_t;
    using vector = std::vector<matrix_value>;

    class Matrix
    {
      public:
        size_t m_width, m_height;
        vector m_values;

      public:
        Matrix(size_t height, size_t width);
        Matrix(size_t height, size_t width, const vector &values);
        Matrix(size_t height, size_t width, const std::vector<vector> &values);
        Matrix(const std::vector<vector> &values);

        Matrix(const Matrix &) = default;
        Matrix(Matrix &&) = default;
        Matrix &operator=(const Matrix &) = default;
        Matrix &operator=(Matrix &&) = default;

      private:
        class Row;
        class Column;
        Row row(size_t i);
        Column column(size_t j);

        void gaussForwards(bool chooseMainElement = true);
        void gaussBackwards();

      public:
        matrix_value &operator()(size_t i, size_t j);
        matrix_value operator()(size_t i, size_t j) const;
        bool isDiagonal() const;

        // void extend(const Matrix &m);
        Matrix fromColumns(size_t from, size_t to);

        void swapRows(size_t i, size_t j);
        void swapColumns(size_t i, size_t j);

        std::pair<Matrix, Matrix> decomposeLU() const;
        Matrix invert() const;
        void normalize();

        Matrix operator*(const Matrix &matrix) const;
        Matrix operator+(const Matrix &matrix) const;
        Matrix operator-(const Matrix &matrix) const;
        matrix_value norm() const;
        matrix_value spectralRadius() const;

        static Matrix id(size_t size);

        friend std::ostream &operator<<(std::ostream &, const Matrix &);
        friend vector solveLinearSystem(const Matrix &, bool);
        friend vector solveLinearSystem(const Matrix &, const vector &, bool);
        friend vector solveLinearSystemUsingLU(const Matrix &matrix, const vector &vector);
        friend vector solveLowerTriangularLinearSystem(const Matrix &extendedMatrix);
        friend vector solveLowerTriangularLinearSystem(const Matrix &matrix, const vector &vector);
        friend vector solveUpperTriangularLinearSystem(const Matrix &extendedMatrix);
        friend vector solveUpperTriangularLinearSystem(const Matrix &matrix, const vector &vector);
    };

    std::ostream &operator<<(std::ostream &stream, const Matrix &m);

    vector solveLinearSystem(const Matrix &extendedMatrix, bool chooseMainElement = true);
    vector solveLinearSystem(const Matrix &matrix, const vector &vector, bool chooseMainElement = true);
    vector solveLinearSystemUsingLU(const Matrix &matrix, const vector &vector);
    vector solveLowerTriangularLinearSystem(const Matrix &extendedMatrix);
    vector solveLowerTriangularLinearSystem(const Matrix &matrix, const vector &vector);
    vector solveUpperTriangularLinearSystem(const Matrix &extendedMatrix);
    vector solveUpperTriangularLinearSystem(const Matrix &matrix, const vector &vector);

    std::vector<std::pair<math::matrix_value, vector>>
    getJacobiEigenvectors(math::Matrix A, long double e, size_t *steps = nullptr);

    const Matrix *for_wolfram(const Matrix &);
} // namespace math

std::ostream &operator<<(std::ostream &stream, const math::vector &v);
std::ostream &operator<<(std::ostream &stream, const math::Matrix *m);
math::matrix_value operator*(const math::vector &k, const math::vector &v);
math::Matrix operator*(math::matrix_value k, math::Matrix m);
math::vector operator*(math::matrix_value k, math::vector v);
math::vector operator/(math::vector v, math::matrix_value k);
math::vector operator*(const math::Matrix &m, const math::vector &v);
math::vector operator+(const math::vector &v1, const math::vector &v2);
math::vector operator-(const math::vector &v1, const math::vector &v2);
math::matrix_value euclideanNorm(const math::vector &v);