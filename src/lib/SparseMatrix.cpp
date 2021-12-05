#include "SparseMatrix.hpp"

#include <algorithm>
#include <fstream>
#include <map>
#include <utility>

#include "DenseMatrix.hpp"

SparseMatrix::SparseMatrix(int height, int width)
    : m_width(width), m_height(height)
{
}

SparseMatrix::SparseMatrix(const std::string &filename)
    : SparseMatrix(std::ifstream(filename))
{
}

SparseMatrix::SparseMatrix(std::istream &&stream)
{
    int width = -1;
    int rowWidth = 0, rowNumber = 0;
    matrix_element_t value;

    std::map<std::pair<int, int>, matrix_element_t> values;

    while (stream >> value, !stream.fail()) {
        if (value) {
            values[{rowNumber, rowWidth}] = value;
        }
        ++rowWidth;

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
        throw std::runtime_error(std::string("Unexpected token '") + c +
                                 "' while reading matrix.");
    }
    if (width < 0) {
        throw std::runtime_error("Stream is empty or doesn't exist");
    }

    m_width = width;
    m_height = rowNumber;
    initFromMap(values);
}

SparseMatrix::SparseMatrix(
    int height, int width,
    const std::map<std::pair<int, int>, matrix_element_t> &values)
    : m_width(width), m_height(height)
{
    initFromMap(values);
}

void SparseMatrix::initFromMap(
    const std::map<std::pair<int, int>, matrix_element_t> &values)
{
    m_rows.reserve(m_height + 1);

    int nnz = 0;
    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            auto it = values.find({i, j});
            if (it != values.end()) {
                m_values.push_back(it->second);
                m_cols.push_back(j);
                ++nnz;
            }
        }
        m_rows.push_back(nnz);
    }
}

const matrix_element_t SparseMatrix::operator()(int i, int j) const
{
    auto rowStart = m_cols.begin() + m_rows[i];
    auto rowEnd = m_cols.begin() + m_rows[i + 1];

    auto it = std::find(rowStart, rowEnd, j);
    if (it == rowEnd) {
        return 0;
    }
    return m_values.begin()[it - m_cols.begin()];
}

int SparseMatrix::getWidth() const
{
    return m_width;
}
int SparseMatrix::getHeight() const
{
    return m_height;
}

std::unique_ptr<Matrix> SparseMatrix::multiply(const Matrix &m) const
{
    if (auto dm = dynamic_cast<const DenseMatrix *>(&m)) {
        return multiply(*dm);
    }
    if (auto sm = dynamic_cast<const SparseMatrix *>(&m)) {
        return multiply(*sm);
    }
    throw std::runtime_error("Unknown matrix type");
}
std::unique_ptr<DenseMatrix> SparseMatrix::multiply(const DenseMatrix &m) const
{
    if (m_width != m.getHeight()) {
        throw std::runtime_error(
            "Impossible to multiply matrices of such dimensions");
    }

    DenseMatrix *result = new DenseMatrix(m_height, m.getWidth());
    for (int r = 0, n = m_height; r < n; r++) {
        for (int i = m_rows[r], l = m_rows[r + 1]; i < l; i++) {
            for (int j = 0; j < m_width; j++) {
                (*result)(r, j) += m_values[i] * m(m_cols[i], j);
            }
        }
    }

    return std::unique_ptr<DenseMatrix>(result);
}
std::unique_ptr<SparseMatrix>
SparseMatrix::multiply(const SparseMatrix &m) const
{
    if (m_width != m.m_height) {
        throw std::runtime_error(
            "Impossible to multiply matrices of such dimensions");
    }

    std::map<std::pair<int, int>, matrix_element_t> values;
    for (int r = 0, n = m_height; r < n; r++) {
        for (int i = m_rows[r], l = m_rows[r + 1]; i < l; i++) {
            for (int j = m.m_rows[m_cols[i]], k = m.m_rows[m_cols[i] + 1];
                 j < k; j++) {
                matrix_element_t value = m_values[i] * m.m_values[j];

                std::pair key(r, m.m_cols[j]);
                auto &&it = values.find(key);
                if (it != values.end()) {
                    it->second += value;
                } else {
                    values[key] = value;
                }
            }
        }
    }

    SparseMatrix *result = new SparseMatrix(m_height, m.getWidth(), values);
    return std::unique_ptr<SparseMatrix>(result);
}