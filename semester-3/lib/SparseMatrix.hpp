#pragma once

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "DenseMatrix.hpp"
#include "Matrix.hpp"

class SparseMatrix : public Matrix
{

  private:
    int m_width, m_height;
    std::vector<matrix_element_t> m_values;
    std::vector<int> m_rows = {0}, m_cols;

    void
    initFromMap(const std::map<std::pair<int, int>, matrix_element_t> &values);

    SparseMatrix(int height, int width,
                 const std::map<std::pair<int, int>, matrix_element_t> &values);
    SparseMatrix(
        int height, int width,
        const std::array<SparseMatrix *, MULT_THREADS_COUNT> &matrices);

  public:
    SparseMatrix(int height, int width);
    SparseMatrix(const std::string &filename);
    SparseMatrix(std::istream &&stream);

    virtual const matrix_element_t operator()(int i, int j) const override;

    virtual int getWidth() const override;
    virtual int getHeight() const override;

  private:
    void multiply(const DenseMatrix &m, int from, int to,
                  DenseMatrix &result) const;
    void multiply(const SparseMatrix &m, int from, int to,
                  SparseMatrix &result) const;

  public:
    virtual std::unique_ptr<Matrix> multiply(const Matrix &m) const override;
    std::unique_ptr<DenseMatrix> multiply(const DenseMatrix &m) const;
    std::unique_ptr<SparseMatrix> multiply(const SparseMatrix &m) const;

    virtual std::unique_ptr<Matrix>
    dmultiply(const Matrix &matrix) const override;
    std::unique_ptr<DenseMatrix> dmultiply(const DenseMatrix &m) const;
    std::unique_ptr<SparseMatrix> dmultiply(const SparseMatrix &m) const;

    friend class DenseMatrix;
    friend bool operator==(const SparseMatrix &m1, const SparseMatrix &m2);
};