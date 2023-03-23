#pragma once
#include <functional>
#include <utility>
#include <vector>

namespace polynomials
{

    using interpolation_table_t = std::vector<std::pair<long double, long double>>;

    interpolation_table_t
    getInterpolationTable(const std::function<long double(long double)> &f, long double m, long double a, long double b);
    interpolation_table_t invertInterpolationTable(const interpolation_table_t &table);
    void sortInterpolationTable(interpolation_table_t &table, long double x);

    std::function<long double(long double)> getLagrangePolynomial(const interpolation_table_t &table, int n);

    namespace interpolateUsing
    {

        long double lagrange(const interpolation_table_t &table, int n, long double x);
        long double newton(const interpolation_table_t &table, int n, long double x);

    } // namespace interpolateUsing

} // namespace polynomials
