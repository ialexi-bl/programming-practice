#include "matrix.hpp"
#include <algorithm>

using namespace math;

static constexpr matrix_value EPSILON = 1e-12;

Matrix::Matrix(int height, int width) : m_width(width), m_height(height), m_values(width * height)
{
}

Matrix::Matrix(int height, int width, std::vector<matrix_value> values) : m_width(width), m_height(height), m_values(values)
{
}

Matrix::Matrix(int height, int width, std::vector<std::vector<matrix_value>> values) : m_width(width), m_height(height)
{
    std::vector<matrix_value> formatted_values;

    for (int j = 0; j < width; j++) {
        for (int i = 0; i < height; i++) {
            formatted_values.push_back(values[i][j]);
        }
    }

    m_values = formatted_values;
}

matrix_value &Matrix::operator()(int i, int j)
{
    return m_values[j * m_height + i];
}

matrix_value Matrix::operator()(int i, int j) const
{
    return m_values[j * m_height + i];
}

static void swap(matrix_value &a, matrix_value &b)
{
    matrix_value t = b;
    b = a;
    a = t;
}

static inline bool eq(matrix_value a, matrix_value b)
{
    return std::abs(a - b) < EPSILON;
}

void Matrix::extend(const Matrix &m)
{
    int s1 = m_height * m_width;
    int s2 = m.m_height * m.m_width;
    m_values.resize(s1 + s2);
    for (int i = 0, s = m.m_height * m.m_width; i < s; i++) {
        m_values[s1 + i] = m.m_values[i];
    }
}

void Matrix::swapRows(int i, int j)
{
    Matrix &self = *this;
    for (int k = 0; k < m_width; k++) {
        swap(self(i, k), self(j, k));
    }
}

void Matrix::swapColumns(int i, int j)
{
    Matrix &self = *this;
    for (int k = 0; k < m_height; k++) {
        swap(self(k, i), self(k, j));
    }
}

std::ostream &math::operator<<(std::ostream &stream, const Matrix &m)
{
    for (int i = 0; i < m.m_height; i++) {
        for (int j = 0; j < m.m_width; j++) {
            stream << m(i, j) << " ";
        }
        stream << std::endl;
    }
    return stream;
}

void Matrix::gaussForwards(bool chooseMainElement)
{
    for (int j = 0; j < m_height; j++) {
        if (chooseMainElement) {
            int mainElementIndex = column(j).mainElementIndex(j);
            if (mainElementIndex != j) {
                swapRows(mainElementIndex, j);
            }
        }

        for (int i = j + 1; i < m_height; i++) {
            if (eq((*this)(i, j), 0)) {
                if (chooseMainElement) {
                    break;
                } else {
                    throw std::runtime_error("Matrix is degenerate");
                }
            }

            row(i) -= row(j) / (*this)(j, j) * (*this)(i, j);
        }
    }
}

void Matrix::gaussBackwards()
{
    for (int j = m_height - 1; j >= 0; j--) {
        // int mainElementIndex = column(j).mainElementIndex(0, j + 1);
        // if (mainElementIndex != j) {swapRows(mainElementIndex, j);}

        for (int i = j - 1; i >= 0; i--) {
            if (eq((*this)(i, j), 0)) {
                continue;
            }

            row(i) -= row(j) / (*this)(j, j) * (*this)(i, j);
        }
    }
}

Matrix Matrix::fromColumns(int from, int to)
{
    Matrix result(to - from, m_height);
    std::copy(m_values.begin() + from * m_height, m_values.begin() + to * m_height, result.m_values.begin());
    return result;
}

std::vector<matrix_value> Matrix::solve(bool chooseMainElement) const
{
    if (m_width != m_height + 1) {
        throw std::runtime_error("Impossible to solve a system with such dimensions");
    }

    Matrix temp = *this;
    temp.gaussForwards(chooseMainElement);

    std::vector<matrix_value> result(m_height);
    for (int i = m_height - 1; i >= 0; i--) {
        matrix_value x = temp(i, m_width - 1);
        for (int j = m_height - 1; j > i; j--) {
            x -= temp(i, j) * result[j];
        }
        result[i] = x / temp(i, i);
    }

    return result;
}

Matrix Matrix::invert() const
{
    if (m_width != m_height) {
        throw std::runtime_error("Non-square matrix doesn't have an inverse");
    }

    Matrix temp(m_height, m_width * 2);
    std::copy(m_values.begin(), m_values.end(), temp.m_values.begin());
    for (int j = 0; j < m_width; j++) {
        for (int i = 0; i < m_height; i++) {
            temp(i, m_width + j) = i == j ? 1 : 0;
        }
    }

    temp.gaussForwards(false);
    for (int i = 0; i < m_height; i++) {
        if (temp(i, i) == 0) {
            throw std::runtime_error("Matrix is degenerate");
        }
    }

    temp.gaussBackwards();
    temp.normalize();
    return temp.fromColumns(m_width, m_width * 2);
}

void Matrix::normalize()
{
    for (int i = 0; i < m_height; i++) {
        if (!eq((*this)(i, i), 0)) {
            row(i) /= (*this)(i, i);
        }
    }
}

// Row

Matrix::Row Matrix::row(int i)
{
    return Row(*this, i);
}

Matrix::Row::Row(Matrix &m, int row) : m_row(row), m_matrix(m)
{
}

Matrix::Row::Row(Matrix &m, int row, matrix_value coef) : m_row(row), m_matrix(m), m_coef(coef)
{
}

Matrix::Row Matrix::Row::operator-() const
{
    return Row(m_matrix, m_row, -m_coef);
}

Matrix::Row Matrix::Row::operator*(matrix_value k) const
{
    return Row(m_matrix, m_row, m_coef * k);
}

Matrix::Row Matrix::Row::operator/(matrix_value k) const
{
    return Row(m_matrix, m_row, m_coef / k);
}

Matrix::Row &Matrix::Row::operator+=(const Row &row)
{
    for (int i = 0; i < m_matrix.m_width; i++) {
        m_matrix(m_row, i) += row(i);
    }
    return *this;
}

Matrix::Row &Matrix::Row::operator-=(const Row &row)
{
    return (*this += -row);
}

Matrix::Row &Matrix::Row::operator*=(matrix_value k)
{
    for (int i = 0; i < m_matrix.m_width; i++) {
        m_matrix(m_row, i) *= k;
    }
    return *this;
}

Matrix::Row &Matrix::Row::operator/=(matrix_value k)
{
    return (*this *= 1 / k);
}

matrix_value Matrix::Row::operator()(int j) const
{
    return m_coef * m_matrix(m_row, j);
}

// Column

Matrix::Column Matrix::column(int j)
{
    return Column(*this, j);
}

Matrix::Column::Column(Matrix &m, int column) : m_column(column), m_matrix(m)
{
}

Matrix::Column::Column(Matrix &m, int column, matrix_value coef) : m_column(column), m_matrix(m), m_coef(coef)
{
}

Matrix::Column Matrix::Column::operator-() const
{
    return Column(m_matrix, m_column, -m_coef);
}

Matrix::Column Matrix::Column::operator*(matrix_value k) const
{
    return Column(m_matrix, m_column, m_coef * k);
}

Matrix::Column Matrix::Column::operator/(matrix_value k) const
{
    return Column(m_matrix, m_column, m_coef / k);
}

Matrix::Column &Matrix::Column::operator+=(const Column &column)
{
    for (int i = 0; i < m_matrix.m_height; i++) {
        m_matrix(i, m_column) += column(i);
    }
    return *this;
}

Matrix::Column &Matrix::Column::operator-=(const Column &column)
{
    return (*this += -column);
}

matrix_value Matrix::Column::operator()(int i) const
{
    return m_coef * m_matrix(i, m_column);
}

int Matrix::Column::mainElementIndex() const
{
    return mainElementIndex(0, m_matrix.m_height);
}

int Matrix::Column::mainElementIndex(int from) const
{
    return mainElementIndex(from, m_matrix.m_height);
}

int Matrix::Column::mainElementIndex(int from, int to) const
{
    int index = from;
    for (int i = from + 1; i < to; i++) {
        if (std::abs(m_matrix(i, m_column)) > std::abs(m_matrix(from, m_column))) {
            index = i;
        }
    }

    return index;
}
