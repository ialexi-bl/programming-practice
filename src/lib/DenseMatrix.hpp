#pragma once

#include <iostream>
#include <memory>
#include <string>

#include "Matrix.hpp"

class DenseMatrix : public Matrix
{
  private:
    int m_width, m_height;
    matrix_element_t *m_matrix;

  public:
    DenseMatrix(int height, int width);
    DenseMatrix(const std::string &filename);
    DenseMatrix(std::istream &&stream);
    ~DenseMatrix();

    matrix_element_t &operator()(int i, int j);
    virtual const matrix_element_t operator()(int i, int j) const override;

    virtual int getWidth() const override;
    virtual int getHeight() const override;

  private:
    void multiply(const DenseMatrix &m, int from, int to,
                  DenseMatrix &result) const;
    void multiply(const SparseMatrix &m, int from, int to,
                  DenseMatrix &result) const;

  public:
    virtual std::unique_ptr<Matrix> multiply(const Matrix &m) const override;
    std::unique_ptr<DenseMatrix> multiply(const DenseMatrix &m) const;
    std::unique_ptr<DenseMatrix> multiply(const SparseMatrix &m) const;

    virtual std::unique_ptr<Matrix> dmultiply(const Matrix &m) const override;
    std::unique_ptr<DenseMatrix> dmultiply(const DenseMatrix &m) const;
    std::unique_ptr<DenseMatrix> dmultiply(const SparseMatrix &m) const;

    friend bool operator==(const DenseMatrix &m1, const DenseMatrix &m2);
};