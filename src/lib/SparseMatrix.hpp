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

  public:
    SparseMatrix(int height, int width);
    SparseMatrix(const std::string &filename);
    SparseMatrix(std::istream &&stream);
    SparseMatrix(int width, int height,
                 const std::map<std::pair<int, int>, matrix_element_t> &values);

    virtual const matrix_element_t operator()(int i, int j) const override;

    virtual int getWidth() const override;
    virtual int getHeight() const override;

    virtual std::unique_ptr<Matrix> multiply(const Matrix &m) const override;
    std::unique_ptr<DenseMatrix> multiply(const DenseMatrix &m) const;
    std::unique_ptr<SparseMatrix> multiply(const SparseMatrix &m) const;

    friend std::unique_ptr<DenseMatrix>
    DenseMatrix::multiply(const SparseMatrix &m) const;
    friend bool operator==(const SparseMatrix &m1, const SparseMatrix &m2);
};