#pragma once
#include <functional>
#include <utility>
#include <vector>

namespace polynomials
{

    using InterpolationTable = std::vector<std::pair<long double, long double>>;

    InterpolationTable
    getInterpolationTable(const std::function<long double(long double)> &f, long double m, long double a, long double b);
    InterpolationTable invertInterpolationTable(const InterpolationTable &table);
    void sortInterpolationTable(InterpolationTable &table, long double x);

    std::function<long double(long double)> getLagrangePolynomial(const InterpolationTable &table, int n);

    namespace interpolateUsing
    {

        long double lagrange(const InterpolationTable &table, int n, long double x);
        long double newton(const InterpolationTable &table, int n, long double x);

    } // namespace interpolateUsing

} // namespace polynomials
