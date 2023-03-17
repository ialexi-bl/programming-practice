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
        int m_width, m_height;
        vector m_values;

      public:
        Matrix(int height, int width);
        Matrix(int height, int width, const vector &values);
        Matrix(int height, int width, const std::vector<vector> &values);

        Matrix(const Matrix &) = default;
        Matrix(Matrix &&) = default;
        Matrix &operator=(const Matrix &) = default;
        Matrix &operator=(Matrix &&) = default;

      private:
        class Row;
        class Column;
        Row row(int i);
        Column column(int j);

        void gaussForwards(bool chooseMainElement = true);
        void gaussBackwards();

      public:
        matrix_value &operator()(int i, int j);
        matrix_value operator()(int i, int j) const;

        // void extend(const Matrix &m);
        Matrix fromColumns(int from, int to);

        void swapRows(int i, int j);
        void swapColumns(int i, int j);

        std::pair<Matrix, Matrix> decomposeLU() const;
        Matrix invert() const;
        void normalize();

        Matrix operator*(const Matrix &matrix) const;
        Matrix operator+(const Matrix &matrix) const;
        Matrix operator-(const Matrix &matrix) const;
        matrix_value norm() const;
        matrix_value spectralRadius() const;

        static Matrix id(int size);

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

    std::vector<std::pair<math::matrix_value, vector>> getJacobiEigenvectors(math::Matrix A, long double e, int *steps = nullptr);

    const Matrix *for_wolfram(const Matrix &);
} // namespace math

std::ostream &operator<<(std::ostream &stream, const math::vector &v);
std::ostream &operator<<(std::ostream &stream, const math::Matrix *m);
math::matrix_value operator*(const math::vector &k, const math::vector &v);
math::Matrix operator*(math::matrix_value k, math::Matrix m);
math::vector operator*(math::matrix_value k, math::vector v);
math::vector operator/(math::vector v, math::matrix_value k);
math::vector operator*(const math::Matrix &m, const math::vector &v);
math::vector operator-(const math::vector &v1, const math::vector &v2);
math::matrix_value euclideanNorm(const math::vector &v);