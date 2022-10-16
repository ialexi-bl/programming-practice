#include <iostream>
#include <utility>
#include <vector>

namespace math
{
    using matrix_value = long double;

    class Matrix
    {
      private:
        int m_width, m_height;
        std::vector<matrix_value> m_values;

      public:
        Matrix(int height, int width);
        Matrix(int height, int width, std::vector<matrix_value> values);
        Matrix(int height, int width, std::vector<std::vector<matrix_value>> values);

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

        void extend(const Matrix &m);
        Matrix fromColumns(int from, int to);

        void swapRows(int i, int j);
        void swapColumns(int i, int j);

        std::pair<Matrix, Matrix> decomposeLU();
        std::vector<matrix_value> solve(bool chooseMainElement = true) const;
        Matrix invert() const;
        void normalize();

        friend std::ostream &operator<<(std::ostream &, const Matrix &);
    };

    std::ostream &operator<<(std::ostream &stream, const Matrix &m);

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
        matrix_value operator()(int i) const;
        int mainElementIndex() const;
        int mainElementIndex(int from) const;
        int mainElementIndex(int from, int to) const;

        friend Matrix;
        friend inline Column operator*(matrix_value k, const Column &column)
        {
            return column * k;
        }
    };

} // namespace math