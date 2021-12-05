#include "DenseMatrix.hpp"

#include <fstream>
#include <list>
#include <stdexcept>

#include "SparseMatrix.hpp"

DenseMatrix::DenseMatrix(int height, int width)
    : m_width(width), m_height(height),
      m_matrix(new matrix_element_t[width * height]())
{
}

DenseMatrix::DenseMatrix(const std::string &filename)
    : DenseMatrix(std::ifstream(filename))
{
}

DenseMatrix::DenseMatrix(std::istream &&stream)
{
    std::list<matrix_element_t> values;

    int width = -1;
    int rowWidth = 0, rowNumber = 0;
    matrix_element_t value;

    while (stream >> value, !stream.fail()) {
        ++rowWidth;
        values.push_back(value);

        while (stream.peek() == ' ')
            stream.get();

        char c = stream.peek();
        if (c != EOF && c != '\n')
            continue;

        if (width == -1) {
            width = rowWidth;
        } else if (width != rowWidth) {
            throw std::runtime_error(
                std::string("Expected row ") + std::to_string(rowNumber) +
                " to contain " + std::to_string(width) + " values, but " +
                std::to_string(rowWidth) + " were found.");
        }

        ++rowNumber;
        rowWidth = 0;
    }
    if (stream.fail() && stream.peek() != EOF) {
        char c = stream.peek();
        throw std::runtime_error(std::string("Unexpected token '") +
                                 (c == EOF ? "EOF" : std::to_string(c)) +
                                 "' while reading matrix.");
    }
    if (width < 0) {
        throw std::runtime_error("Stream is empty or doesn't exist");
    }

    m_width = width;
    m_height = rowNumber;
    m_matrix = new matrix_element_t[width * rowNumber];
    int i = 0, j = 0;
    for (matrix_element_t value : values) {
        (*this)(i, j) = value;

        ++j;
        i += j / width;
        j %= width;
    }
}

DenseMatrix::~DenseMatrix()
{
    delete[] m_matrix;
}

matrix_element_t &DenseMatrix::operator()(int i, int j)
{
    return m_matrix[i * m_width + j];
}

const matrix_element_t DenseMatrix::operator()(int i, int j) const
{
    return m_matrix[i * m_width + j];
}

int DenseMatrix::getWidth() const
{
    return m_width;
}
int DenseMatrix::getHeight() const
{
    return m_height;
}

std::unique_ptr<Matrix> DenseMatrix::multiply(const Matrix &m) const
{
    if (auto dm = dynamic_cast<const DenseMatrix *>(&m)) {
        return multiply(*dm);
    }
    if (auto sm = dynamic_cast<const SparseMatrix *>(&m)) {
        return multiply(*sm);
    }
    throw std::runtime_error("Unknown matrix type");
}

std::unique_ptr<DenseMatrix> DenseMatrix::multiply(const DenseMatrix &m) const
{
    if (m_width != m.m_height) {
        throw std::runtime_error(
            "Impossible to multiply matrices of such dimensions");
    }

    DenseMatrix *result = new DenseMatrix(m_height, m.m_width);
    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m.m_width; j++) {
            for (int k = 0; k < m_width; k++) {
                (*result)(i, j) += (*this)(i, k) * m(k, j);
            }
        }
    }

    return std::unique_ptr<DenseMatrix>(result);
}

std::unique_ptr<DenseMatrix> DenseMatrix::multiply(const SparseMatrix &m) const
{
    if (m_width != m.m_height) {
        throw std::runtime_error(
            "Impossible to multiply matrices of such dimensions");
    }

    DenseMatrix *result = new DenseMatrix(m_height, m.m_width);
    for (int r = 0, n = m.m_height; r < n; r++) {
        for (int j = m.m_rows[r], l = m.m_rows[r + 1]; j < l; j++) {
            for (int i = 0; i < m_height; i++) {
                (*result)(i, m.m_cols[j]) += (*this)(i, r) * m.m_values[j];
            }
        }
    }

    return std::unique_ptr<DenseMatrix>(result);
}
