#include "polynomials.hpp"
#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>

namespace polynomials
{

InterpolationTable getInterpolationTable(long double (*f)(long double),
                                         long double m, long double a,
                                         long double b)
{
    InterpolationTable result;
    result.reserve(m + 1);

    long double h = (b - a) / m;

    for (int i = 0; i <= m; i++) {
        long double x = a + i * h;
        result.push_back({x, f(x)});
    }

    return result;
}

void sortInterpolationTable(InterpolationTable &table, long double x)
{
    std::sort(table.begin(), table.end(),
              [x](const std::pair<long double, long double> &a,
                  const std::pair<long double, long double> &b) {
                  return abs(a.first - x) < abs(b.first - x);
              });
}

namespace interpolateUsing
{

static long double getLagrangeCoef(const InterpolationTable &table, int n,
                                   int k, long double x)
{
    long double numerator = 1, denominator = 1;
    long double x_k = table[k].first;
    for (int i = 0; i <= n; i++) {
        if (i == k) continue;

        numerator *= x - table[i].first;
        denominator *= x_k - table[i].first;
    }
    return numerator / denominator;
}

long double lagrange(const InterpolationTable &table, int n, long double x)
{
    long double result = 0;
    for (int i = 0; i <= n; i++) {
        result += getLagrangeCoef(table, n, i, x) * table[i].second;
    }
    return result;
}

long double newton(const InterpolationTable &table, int n, long double x)
{
    long double result = 0, coef = 1;
    std::vector<long double> differences;
    differences.reserve(n + 1);

    for (int i = 0; i <= n; i++) differences.push_back(table[i].second);
    for (int i = 0; i <= n; i++) {
        result += differences[0] * coef;
        coef *= x - table[i].first;

        for (int j = 0; j < n - i; j++) {
            differences[j] = (differences[j + 1] - differences[j]) /
                             (table[j + i + 1].first - table[j].first);
        }
    }

    return result;
}

} // namespace interpolateUsing

} // namespace polynomials