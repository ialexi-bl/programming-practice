#include "Matrix.hpp"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>

#include "DenseMatrix.hpp"
#include "SparseMatrix.hpp"

Matrix::~Matrix()
{
}

std::string Matrix::toString() const
{
    std::ostringstream stream;
    stream << *this;
    return stream.str();
}

std::ostream &operator<<(std::ostream &stream, const Matrix &matrix)
{
    for (int i = 0, m = matrix.getWidth(); i < m; i++) {
        for (int j = 0, n = matrix.getHeight(); j < n; j++) {
            stream << matrix(i, j) << ' ';
        }
        stream << '\n';
    }
    return stream;
}

std::unique_ptr<Matrix> Matrix::add(const Matrix &matrix) const
{
    if (matrix.getHeight() != getHeight() || matrix.getWidth() != getWidth()) {
        throw std::runtime_error("Impossible to add matrices of different dimensions");
    }

    DenseMatrix *result = new DenseMatrix(getHeight(), getWidth());
    for (int i = 0, m = matrix.getWidth(); i < m; i++) {
        for (int j = 0, n = matrix.getHeight(); j < n; j++) {
            (*result)(i, j) = (*this)(i, j) + matrix(i, j);
        }
    }

    return std::unique_ptr<Matrix>(result);
}

// Comparison

bool operator==(const Matrix &m1, const Matrix &m2)
{
    if (&m1 == &m2) {
        return true;
    }

    if (auto dm1 = dynamic_cast<const DenseMatrix *>(&m1)) {
        if (auto dm2 = dynamic_cast<const DenseMatrix *>(&m2)) {
            return *dm1 == *dm2;
        }
    } else if (auto sm1 = dynamic_cast<const SparseMatrix *>(&m1)) {
        if (auto sm2 = dynamic_cast<const SparseMatrix *>(&m2)) {
            return *sm1 == *sm2;
        }
    }
    return false;
}

bool operator==(const DenseMatrix &m1, const DenseMatrix &m2)
{
    if (m1.getWidth() != m2.getWidth() || m1.getHeight() != m2.getHeight()) {
        return false;
    }
    for (int i = 0, m = m1.getWidth() * m1.getHeight(); i < m; i++) {
        if (std::abs(m1.m_matrix[i] - m2.m_matrix[i]) > 1e-6) {
            return false;
        }
    }
    return true;
}

bool operator==(const SparseMatrix &m1, const SparseMatrix &m2)
{
    if (m1.getWidth() != m2.getWidth() || m1.getHeight() != m2.getHeight() || m1.m_values.size() != m2.m_values.size()) {
        return false;
    }
    for (int i = 0, m = m1.m_rows.size(); i < m; i++) {
        if (m1.m_rows[i] != m2.m_rows[i]) {
            return false;
        }
    }
    for (int i = 0, m = m1.m_cols.size(); i < m; i++) {
        if (m1.m_cols[i] != m2.m_cols[i] || std::abs(m1.m_values[i] - m2.m_values[i]) > 1e-6) {
            return false;
        }
    }
    return true;
}
