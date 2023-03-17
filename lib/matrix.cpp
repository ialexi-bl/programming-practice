#include "matrix.hpp"
#include <algorithm>
#include <cmath>

std::ostream &operator<<(std::ostream &stream, const math::vector &v)
{
    stream << "(";
    for (size_t i = 0; i < v.size(); i++) {
        if (i) {
            stream << ", ";
        }
        stream << v[i];
    }
    stream << ")";
    return stream;
}

math::vector operator*(math::matrix_value k, math::vector v)
{
    for (size_t i = 0; i < v.size(); i++) {
        v[i] *= k;
    }
    return v;
}

math::Matrix operator*(math::matrix_value k, math::Matrix m)
{
    for (int i = 0; i < m.m_height; i++) {
        for (int j = 0; j < m.m_width; j++) {
            m(i, j) *= k;
        }
    }
    return m;
}

math::vector operator/(math::vector v, math::matrix_value k)
{
    for (size_t i = 0; i < v.size(); i++) {
        v[i] /= k;
    }
    return v;
}

math::vector operator*(const math::Matrix &m, const math::vector &v)
{
    if (m.m_width != static_cast<int>(v.size())) {
        throw std::runtime_error("Impossible to multiply such matrix and vector");
    }

    math::vector result(m.m_height);

    for (int i = 0; i < m.m_height; i++) {
        for (int j = 0; j < m.m_width; j++) {
            result[i] += m(i, j) * v[j];
        }
    }

    return result;
}

math::matrix_value operator*(const math::vector &v1, const math::vector &v2)
{
    math::matrix_value result = 0;
    for (size_t i = 0; i < v2.size(); i++) {
        result += v1[i] * v2[i];
    }
    return result;
}

math::vector operator-(const math::vector &v1, const math::vector &v2)
{
    math::vector result = v1;
    for (size_t i = 0; i < result.size(); i++) {
        result[i] -= v2[i];
    }
    return result;
}

math::matrix_value euclideanNorm(const math::vector &v)
{
    return std::sqrt(v * v);
}

namespace math
{

    static constexpr matrix_value EPSILON = 1e-12;

    class Matrix::Row
    {

      private:
        int m_row;
        Matrix &m_matrix;
        matrix_value m_coef = 1;

      private:
        Row(Matrix &m, int row);
        Row(Matrix &m, int row, matrix_value coef);

        Row operator-() const;
        Row operator*(matrix_value k) const;
        Row operator/(matrix_value k) const;
        Row &operator+=(const Row &row);
        Row &operator-=(const Row &row);
        Row &operator*=(matrix_value k);
        Row &operator/=(matrix_value k);
        matrix_value operator()(int j) const;

        friend Matrix;
        friend inline Row operator*(matrix_value k, const Row &&row)
        {
            return row * k;
        }
    };

    class Matrix::Column
    {

      private:
        int m_column;
        Matrix &m_matrix;

        matrix_value m_coef = 1;

      private:
        Column(Matrix &m, int column);
        Column(Matrix &m, int column, matrix_value coef);

        Column operator-() const;
        Column operator*(matrix_value k) const;
        Column operator/(matrix_value k) const;
        Column &operator+=(const Column &column);
        Column &operator-=(const Column &column);
        matrix_value operator()(size_t i) const;
        int mainElementIndex() const;
        int mainElementIndex(int from) const;
        int mainElementIndex(int from, int to) const;

        friend Matrix;
        friend inline Column operator*(matrix_value k, const Column &column)
        {
            return column * k;
        }
    };

    Matrix::Matrix(int height, int width) : m_width(width), m_height(height), m_values(width * height)
    {
    }

    Matrix::Matrix(int height, int width, const vector &values) : m_width(width), m_height(height)
    {
        m_values = values;
    }

    Matrix::Matrix(int height, int width, const std::vector<vector> &values) : m_width(width), m_height(height)
    {
        vector formatted_values;

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

    std::ostream &operator<<(std::ostream &stream, const Matrix &m)
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

            for (size_t i = j - 1; i >= 0; i--) {
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

    std::pair<Matrix, Matrix> Matrix::decomposeLU() const
    {
        if (m_height != m_width) {
            throw std::runtime_error("Impossible to LU decompose matrix with such dimensions");
        }

        std::pair<Matrix, Matrix> lu {Matrix(m_height, m_width), Matrix(m_height, m_width)};
        for (int i = 0; i < m_width; i++) {
            lu.second(i, i) = 1;
        }

        for (int i = 0; i < m_width; i++) {
            for (int j = i; j < m_height; j++) {
                lu.first(j, i) = (*this)(j, i);

                for (int k = 0; k <= i - 1; k++) {
                    lu.first(j, i) -= lu.first(j, k) * lu.second(k, i);
                }
            }
            for (int j = i + 1; j < m_height; j++) {
                lu.second(i, j) = (*this)(i, j);

                for (int k = 0; k <= i - 1; k++) {
                    lu.second(i, j) -= lu.first(i, k) * lu.second(k, j);
                }
                lu.second(i, j) /= lu.first(i, i);
            }
        }

        return lu;
    }

    Matrix Matrix::operator*(const Matrix &m) const
    {
        if (m_width != m.m_height) {
            throw std::runtime_error("Impossible to multiply such matrices");
        }

        Matrix result(m_height, m.m_width);

        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m.m_width; j++) {
                for (int k = 0; k < m_width; k++) {
                    result(i, j) += (*this)(i, k) * m(k, j);
                }
            }
        }

        return result;
    }

    Matrix Matrix::operator+(const Matrix &m) const
    {
        if (m_width != m.m_width || m_height != m.m_height) {
            throw std::runtime_error("Impossible to add such matrices");
        }

        Matrix result(m_height, m_width);
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                result(i, j) = (*this)(i, j) + m(i, j);
            }
        }

        return result;
    }

    Matrix Matrix::operator-(const Matrix &m) const
    {
        if (m_width != m.m_width || m_height != m.m_height) {
            throw std::runtime_error("Impossible to subtract such matrices");
        }

        Matrix result(m_height, m_width);
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                result(i, j) = (*this)(i, j) - m(i, j);
            }
        }

        return result;
    }

    matrix_value Matrix::norm() const
    {
        matrix_value result = -std::numeric_limits<matrix_value>::infinity();

        for (int i = 0; i < m_height; i++) {
            matrix_value sum = 0;
            for (int j = 0; j < m_width; j++) {
                sum += std::abs((*this)(i, j));
            }
            if (sum > result) {
                result = sum;
            }
        }

        return result;
    }

    static math::matrix_value scalarProduct(const math::vector &v1, const math::vector &v2)
    {
        math::matrix_value result = 0;
        for (size_t i = 0; i < v1.size(); i++) {
            result += v1[i] * v2[i];
        }
        return result;
    }

    matrix_value Matrix::spectralRadius() const
    {
        Matrix xCurrent(m_height, 1);
        Matrix xNext(m_height, 1);
        for (int i = 0; i < m_height; i++) {
            xCurrent(i, 0) = 1;
        }

        matrix_value lambda = 0;
        for (int k = 0; k < 50; k++) {
            xNext = (*this) * xCurrent;
            lambda = scalarProduct(xNext.m_values, xCurrent.m_values) / scalarProduct(xCurrent.m_values, xCurrent.m_values);
            xCurrent = xNext;
        }

        return std::abs(lambda);
    }

    Matrix Matrix::id(int size)
    {
        Matrix E(size, size);
        for (int i = 0; i < size; i++) {
            E(i, i) = 1;
        }
        return E;
    }

    vector solveLinearSystem(const Matrix &matrix, const vector &vc, bool chooseMainElement)
    {
        if (static_cast<int>(vc.size()) != matrix.m_height) {
            throw std::runtime_error("Impossible to solve a system with such dimensions");
        }

        auto v = matrix.m_values;
        v.insert(v.end(), vc.begin(), vc.end());
        Matrix temp(matrix.m_height, matrix.m_height + 1, v);
        return solveLinearSystem(temp, chooseMainElement);
    }

    vector solveLinearSystem(const Matrix &extendedMatrix, bool chooseMainElement)
    {
        if (extendedMatrix.m_width != extendedMatrix.m_height + 1) {
            throw std::runtime_error("Impossible to solve a system with such dimensions");
        }

        Matrix temp = extendedMatrix;
        temp.gaussForwards(chooseMainElement);

        return solveUpperTriangularLinearSystem(temp);
    }

    vector solveLinearSystemUsingLU(const Matrix &matrix, const vector &v)
    {
        if (static_cast<int>(v.size()) != matrix.m_height) {
            throw std::runtime_error("Impossible to solve a system with such dimensions");
        }

        const std::pair<Matrix, Matrix> lu = matrix.decomposeLU();
        const Matrix &L = lu.first;
        const Matrix &U = lu.second;

        std::cout << L << std::endl;
        std::cout << U << std::endl;

        for (int i = 0; i < L.m_height; i++) {
            if (L(i, i) == 0) {
                throw std::runtime_error("Matrix is degenerate");
            }
        }

        // std::cout << "1" << std::endl;
        vector lResult = solveLowerTriangularLinearSystem(L, v);
        vector uResult = solveUpperTriangularLinearSystem(U, lResult);
        // std::cout << "2" << std::endl;
        return uResult;
    }

    vector solveLowerTriangularLinearSystem(const Matrix &matrix, const vector &vc)
    {
        if (static_cast<int>(vc.size()) != matrix.m_height) {
            throw std::runtime_error("Impossible to solve a system with such dimensions");
        }

        vector v;
        v.insert(v.end(), matrix.m_values.begin(), matrix.m_values.end());
        v.insert(v.end(), vc.begin(), vc.end());
        Matrix temp(matrix.m_height, matrix.m_height + 1, v);
        std::cout << "a" << std::endl;
        auto b = solveLowerTriangularLinearSystem(temp);
        std::cout << "b" << std::endl;
        return b;
    }

    vector solveLowerTriangularLinearSystem(const Matrix &extendedMatrix)
    {
        if (extendedMatrix.m_width != extendedMatrix.m_height + 1) {
            throw std::runtime_error("Impossible to solve a system with such dimensions");
        }
        for (int i = 0; i < extendedMatrix.m_height; i++) {
            if (extendedMatrix(i, i) == 0) {
                throw std::runtime_error("Matrix is degenerate");
            }
        }

        Matrix temp = extendedMatrix;

        vector result(temp.m_height);
        for (int i = 0; i <= temp.m_height; i++) {
            matrix_value x = temp(i, temp.m_width - 1);
            for (int j = 0; j < i; j++) {
                x -= temp(i, j) * result[j];
            }
            result[i] = x / temp(i, i);
        }

        return result;
    }

    vector solveUpperTriangularLinearSystem(const Matrix &extendedMatrix)
    {
        if (extendedMatrix.m_width != extendedMatrix.m_height + 1) {
            throw std::runtime_error("Impossible to solve a system with such dimensions");
        }
        for (int i = 0; i < extendedMatrix.m_height; i++) {
            if (extendedMatrix(i, i) == 0) {
                throw std::runtime_error("Matrix is degenerate");
            }
        }

        Matrix temp = extendedMatrix;

        vector result(temp.m_height);
        for (int i = temp.m_height - 1; i >= 0; i--) {
            matrix_value x = temp(i, temp.m_width - 1);
            for (int j = temp.m_height - 1; j > i; j--) {
                x -= temp(i, j) * result[j];
            }
            result[i] = x / temp(i, i);
        }

        return result;
    }

    vector solveUpperTriangularLinearSystem(const Matrix &matrix, const vector &vc)
    {
        if (static_cast<int>(vc.size()) != matrix.m_height) {
            throw std::runtime_error("Impossible to solve a system with such dimensions");
        }

        auto v = matrix.m_values;
        v.insert(v.end(), vc.begin(), vc.end());
        Matrix temp(matrix.m_height, matrix.m_height + 1, v);
        return solveUpperTriangularLinearSystem(temp);
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

    matrix_value Matrix::Column::operator()(size_t i) const
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

    static std::pair<int, int> getLargestAboveDiagonal(const math::Matrix &matrix)
    {
        int ir = 0, jr = 1;
        for (int j = 2; j < matrix.m_width; j++) {
            for (int i = 0; i < j; i++) {
                if (std::abs(matrix(i, j)) > std::abs(matrix(ir, jr))) {
                    ir = i;
                    jr = j;
                }
            }
        }

        return {ir, jr};
    }

    std::vector<std::pair<math::matrix_value, math::vector>> getJacobiEigenvectors(math::Matrix A, long double e, int *steps)
    {
        auto X = math::Matrix::id(A.m_height);

        if (steps) {
            steps = 0;
        }
        while (true) {
            auto [ik, jk] = getLargestAboveDiagonal(A);

            if (std::abs(A(ik, jk)) < e) {
                break;
            }

            if (steps) {
                steps++;
            }

            auto d = std::sqrt((A(ik, ik) - A(jk, jk)) * (A(ik, ik) - A(jk, jk)) + 4.0 * A(ik, jk) * A(ik, jk));
            auto c = std::sqrt((1.0 / 2.0) * (1 + std::abs(A(ik, ik) - A(jk, jk)) / d));
            auto s = sign(A(ik, jk)) * sign(A(ik, ik) - A(jk, jk)) *
                     std::sqrt((1.0 / 2.0) * (1.0 - std::abs(A(ik, ik) - A(jk, jk)) / d));

            math::vector newValuesI(A.m_height);
            for (int i = 0; i < A.m_height; i++) {
                if (i == ik || i == jk) {
                    continue;
                }
                // A(i, ik) = A(ik, i) = c * A(i, ik) + s * A(i, jk);
                newValuesI[i] = c * A(i, ik) + s * A(i, jk);
            }
            newValuesI[ik] = c * c * A(ik, ik) + 2 * c * s * A(ik, jk) + s * s * A(jk, jk);
            newValuesI[jk] = 0;

            math::vector newValuesJ(A.m_height);
            for (int i = 0; i < A.m_height; i++) {
                if (i == ik || i == jk) {
                    continue;
                }
                // A(i, jk) = A(jk, i) = c * A(i, jk) - s * A(i, ik);
                newValuesJ[i] = c * A(i, jk) - s * A(i, ik);
            }
            newValuesJ[ik] = 0;
            newValuesJ[jk] = s * s * A(ik, ik) - 2 * c * s * A(ik, jk) + c * c * A(jk, jk);

            for (int i = 0; i < A.m_height; i++) {
                A(i, ik) = A(ik, i) = newValuesI[i];
                A(i, jk) = A(jk, i) = newValuesJ[i];
            }

            for (int i = 0; i < A.m_width; i++) {
                if (i == ik || i == jk) {
                    continue;
                }
                auto xii = X(i, ik) * c + X(i, jk) * s;
                auto xij = -X(i, ik) * s + X(i, jk) * c;
                X(i, ik) = xii;
                X(i, jk) = xij;
            }
            auto xii = X(ik, ik) * c + X(ik, jk) * s;
            auto xij = -X(ik, ik) * s + X(ik, jk) * c;
            auto xji = X(jk, ik) * c + X(jk, jk) * s;
            auto xjj = -X(jk, ik) * s + X(jk, jk) * c;
            X(ik, ik) = xii;
            X(ik, jk) = xij;
            X(jk, ik) = xji;
            X(jk, jk) = xjj;
        }

        std::vector<std::pair<math::matrix_value, math::vector>> result;
        for (int i = 0; i < A.m_width; i++) {
            math::vector eigenvector(A.m_height);
            for (int j = 0; j < A.m_height; j++) {
                eigenvector[j] = X(j, i);
            }

            result.push_back({A(i, i), eigenvector});
        }

        return result;
    }

    const math::Matrix *for_wolfram(const math::Matrix &m)
    {
        return &m;
    }

} // namespace math

std::ostream &operator<<(std::ostream &stream, const math::Matrix *m)
{
    stream << "{";
    for (size_t i = 0; i < m->m_height; i++) {
        if (i) {
            stream << ",";
        }
        stream << "{";
        for (size_t j = 0; j < m->m_width; j++) {
            if (j) {
                stream << ",";
            }
            stream << (*m)(i, j);
        }
        stream << "}";
    }
    stream << "}";
    return stream;
}