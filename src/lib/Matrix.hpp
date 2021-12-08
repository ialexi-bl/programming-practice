#pragma once

#include <iostream>
#include <memory>
#include <string>

using matrix_element_t = double;

constexpr unsigned int MULT_THREADS_COUNT = 8;

class DenseMatrix;
class SparseMatrix;

class Matrix
{
  public:
    virtual ~Matrix();

    virtual const matrix_element_t operator()(int i, int j) const = 0;

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;

    virtual std::unique_ptr<Matrix> multiply(const Matrix &matrix) const = 0;
    virtual std::unique_ptr<Matrix> dmultiply(const Matrix &matrix) const = 0;

    std::string toString() const;
};

std::ostream &operator<<(std::ostream &stream, const Matrix &matrix);

bool operator==(const DenseMatrix &m1, const DenseMatrix &m2);
bool operator==(const SparseMatrix &m1, const SparseMatrix &m2);
bool operator==(const Matrix &m1, const Matrix &m2);
