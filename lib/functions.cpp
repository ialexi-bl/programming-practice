#include "functions.hpp"
#include "math.hpp"
#include "roots.hpp"
#include <array>
#include <assert.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <numbers>

namespace math
{

    static std::vector<value_t> getLegendreRoots(int n)
    {
        return findAllRootsUsing::tangents(polynomials::legendre::get(n), -1, 1, 1e-15, 1e5);
    }

    value_t getGaussCoefficient(int n, value_t a, value_t b, value_t xk)
    {
        assert(n > 0 || "Unable to compute gauss coefficients for n <= 0");

        value_t q = (b - a) / 2;
        value_t p = polynomials::legendre::evaluate(n - 1, xk);
        return q * 2.0 * (1.0 - xk * xk) / (n * n * p * p);
    }

    value_t getGaussCoefficient(int n, value_t xk)
    {
        return getGaussCoefficient(n, -1, 1, xk);
    }

    std::vector<std::pair<value_t, value_t>> getGaussCoefficients(int n, value_t a, value_t b)
    {
        std::vector<value_t> roots = getLegendreRoots(n);
        std::vector<std::pair<value_t, value_t>> result;

        value_t q = (b - a) / 2;
        for (value_t root : roots) {
            result.push_back({a + q * (root + 1), getGaussCoefficient(n, a, b, root)});
        }

        return result;
    }

    std::vector<std::pair<value_t, value_t>> getGaussCoefficients(int n)
    {
        return getGaussCoefficients(n, -1, 1);
    }

    std::vector<value_t> calculateDerivativeValues(simple_function_t f, value_t a, value_t b, int m)
    {
        assert(m >= 2 || "Cannot calculate derivative for less than 3 points");

        std::vector<value_t> result;
        result.reserve(m + 1);

        value_t h = (b - a) / m;
        result.push_back((-3 * f(a) + 4 * f(a + h) - f(a + 2 * h)) / (2 * h));

        for (int i = 1; i < m; i++) {
            value_t x = a + i * h;
            result.push_back((f(x + h) - f(x - h)) / (2 * h));
        }

        result.push_back((3 * f(b) - 4 * f(b - h) + f(b - 2 * h)) / (2 * h));

        return result;
    }
    std::vector<value_t> calculateDerivativeValuesPrecise(simple_function_t f, value_t a, value_t b, int m)
    {
        assert(m >= 3 || "Cannot calculate precise derivative for less than 4 points");

        std::vector<value_t> result;
        result.reserve(m + 1);

        value_t h = (b - a) / m;
        result.push_back((-11 * f(a) + 18 * f(a + h) - 9 * f(a + 2 * h) + 2 * f(a + 3 * h)) / (6 * h));
        result.push_back((f(a + 2 * h) - f(a)) / (2 * h));

        for (int i = 2; i < m - 1; i++) {
            value_t x = a + i * h;
            result.push_back((f(x - 2 * h) - 8 * f(x - h) + 8 * f(x + h) - f(x + 2 * h)) / (12 * h));
        }

        result.push_back((f(b) - f(b - 2 * h)) / (2 * h));
        result.push_back((11 * f(b) - 18 * f(b - h) + 9 * f(b - 2 * h) - 2 * f(b - 3 * h)) / (6 * h));

        return result;
    }
    std::vector<value_t> calculateSecondDerivativeValues(simple_function_t f, value_t a, value_t b, int m)
    {
        std::vector<value_t> result;
        result.reserve(m + 1);
        result.push_back(std::nanl("impossible"));

        value_t h = (b - a) / m;
        for (int i = 1; i < m; i++) {
            value_t x = a + i * h;
            result.push_back((f(x - h) - 2 * f(x) + f(x + h)) / (h * h));
        }

        result.push_back(std::nanl("impossible"));
        return result;
    }

    value_t getAbsMaxOnInterval(std::vector<value_t> maxima, simple_function_t f, value_t a, value_t b)
    {
        value_t result = std::max(std::abs(f(a)), std::abs(f(b)));

        for (value_t maximum : maxima) {
            if (a <= maximum && maximum <= b) {
                result = std::max(result, std::abs(f(maximum)));
            }
        }
        return result;
    }

    value_t calculateMoment(simple_function_t w, value_t a, value_t b, int k)
    {
        return calculateIntegralUsing::compound::middleRect(
            [w, k](value_t x) -> value_t {
                return w(x) * std::pow(x, k);
            },
            a,
            b,
            10000
        );
    }
    std::vector<value_t> calculateMoments(simple_function_t w, value_t a, value_t b, int n)
    {
        std::vector<value_t> result;
        result.reserve(n);
        for (int k = 0; k < n; k++) result.push_back(calculateMoment(w, a, b, k));
        return result;
    }

    static std::pair<value_t, value_t>
    solveSecondOrderLinearSystem(const std::array<value_t, 4> &coefs, const std::pair<value_t, value_t> &constant_terms)
    {
        value_t D = coefs[0] * coefs[3] - coefs[1] * coefs[2];

        if (D == 0) {
            throw std::runtime_error("Attempted to solve a linear system with zero determinant");
        }

        value_t D1 = constant_terms.first * coefs[3] - coefs[1] * constant_terms.second;
        value_t D2 = coefs[0] * constant_terms.second - constant_terms.first * coefs[2];

        return {D1 / D, D2 / D};
    }

    polynomials::Polynomial getOrthogonalPolynomial(simple_function_t w, value_t a, value_t b)
    {
        const std::vector<value_t> moments = calculateMoments(w, a, b, 4);
        const std::pair<value_t, value_t> coefs =
            solveSecondOrderLinearSystem({moments[1], moments[0], moments[2], moments[1]}, {-moments[2], -moments[3]});

        return {{coefs.second, coefs.first, 1}};
    }

    static std::pair<value_t, value_t> getOrthogonalPolynomialRoots(simple_function_t w, value_t a, value_t b)
    {
        const polynomials::Polynomial polynomial = getOrthogonalPolynomial(w, a, b);
        value_t D = std::sqrt(polynomial[1] * polynomial[1] - 4 * polynomial[0] * polynomial[2]);
        return {(-polynomial[1] - D) / 2, (-polynomial[1] + D) / 2};
    }

    std::vector<std::pair<value_t, value_t>> getArbitraryWeightCoefs(simple_function_t w, value_t a, value_t b)
    {
        const std::pair<value_t, value_t> roots = getOrthogonalPolynomialRoots(w, a, b);
        const std::pair<value_t, value_t> coefs = solveSecondOrderLinearSystem(
            {1, 1, roots.first, roots.second},
            {calculateMoment(w, a, b, 0), calculateMoment(w, a, b, 1)}
        );
        return {{roots.first, coefs.first}, {roots.second, coefs.second}};
    }

    value_t calculateArbitraryWeightIntegral(simple_function_t f, simple_function_t w, value_t a, value_t b)
    {
        std::vector<std::pair<value_t, value_t>> coefs = getArbitraryWeightCoefs(w, a, b);

        value_t result = 0;
        for (int i = 0, s = coefs.size(); i < s; i++) {
            result += coefs[i].second * f(coefs[i].first);
        }
        return result;
    }

    namespace calculateIntegralUsing
    {
        value_t leftRect(simple_function_t f, value_t a, value_t b)
        {
            return (b - a) * f(a);
        }
        value_t middleRect(simple_function_t f, value_t a, value_t b)
        {
            value_t mid = a + (b - a) / 2;
            return (b - a) * f(mid);
        }
        value_t rightRect(simple_function_t f, value_t a, value_t b)
        {
            return (b - a) * f(b);
        }
        value_t trapezoid(simple_function_t f, value_t a, value_t b)
        {
            return (b - a) * (f(a) + f(b)) / 2;
        }
        value_t simpson(simple_function_t f, value_t a, value_t b)
        {
            value_t mid = a + (b - a) / 2;
            return (b - a) * (f(a) + 4 * f(mid) + f(b)) / 6;
        }
        value_t threeEights(simple_function_t f, value_t a, value_t b)
        {
            value_t h = (b - a) / 3;
            return (b - a) * (f(a) + 3 * f(a + h) + 3 * f(b - h) + f(b)) / 8;
        }
        value_t gauss(simple_function_t f, int n, value_t a, value_t b)
        {
            const std::vector<std::pair<value_t, value_t>> coefs = getGaussCoefficients(n, a, b);
            value_t result = 0;
            for (auto &&it : coefs) {
                result += it.second * f(it.first);
            }

            return result;
        }

        static std::vector<std::pair<value_t, value_t>> get_meler_coefficients(int n, value_t a, value_t b)
        {
            value_t q = (b - a) / 2;

            std::vector<std::pair<value_t, value_t>> result;
            result.reserve(n);

            value_t c = q * std::numbers::pi / n;
            for (int k = 1; k <= n; k++) {
                value_t xk = std::cos((2 * k - 1) * std::numbers::pi / (2 * n));
                result.push_back({a + q * (xk + 1), c});
            }

            return result;
        }
        value_t meler(simple_function_t f, int n, value_t a, value_t b)
        {
            std::vector<std::pair<value_t, value_t>> coefs = get_meler_coefficients(n, a, b);

            value_t result = 0;
            for (auto &&it : coefs) {
                result += it.second * f(it.first);
            }

            return result;
        }

        namespace compound
        {
            static value_t applyCompoundFormula(
                simple_function_t f,
                value_t a,
                value_t b,
                int m,
                std::function<value_t(simple_function_t, value_t, value_t)> applyFormula
            )
            {
                value_t result = 0, h = (b - a) / m;
                for (int i = 1; i <= m; i++) {
                    result += applyFormula(f, a + h * (i - 1), a + h * i);
                }
                return result;
            }

            value_t leftRect(simple_function_t f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::leftRect);
            }
            value_t middleRect(simple_function_t f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::middleRect);
            }
            value_t rightRect(simple_function_t f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::rightRect);
            }
            value_t trapezoid(simple_function_t f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::trapezoid);
            }
            value_t simpson(simple_function_t f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::simpson);
            }
            value_t threeEights(simple_function_t f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::threeEights);
            }
            value_t gauss(simple_function_t f, int n, value_t a, value_t b, int m, integrator_t integrate)
            {
                const std::vector<std::pair<value_t, value_t>> coefs = getGaussCoefficients(n);
                return applyCompoundFormula(
                    f,
                    a,
                    b,
                    m,
                    [integrate, &coefs](simple_function_t f, value_t c, value_t d) -> value_t {
                        value_t result = 0;
                        value_t q = (d - c) / 2;
                        for (auto &&it : coefs) {
                            result += q * it.second * f(c + (it.first + 1) * q);
                        }

                        return result;
                    }
                );
            }
        } // namespace compound
    }     // namespace calculateIntegralUsing

    namespace getTheoreticalErrorOf
    {
        namespace compound
        {
            value_t leftRect(extr_calculator_t getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 2) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 1);
            }
            value_t middleRect(extr_calculator_t getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 2) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 2);
            }
            value_t rightRect(extr_calculator_t getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 2) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 1);
            }
            value_t trapezoid(extr_calculator_t getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 12) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 2);
            }
            value_t simpson(extr_calculator_t getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 24) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 4);
            }
            value_t threeEights(extr_calculator_t getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 2880) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 4);
            }
        } // namespace compound
    }     // namespace getTheoreticalErrorOf

} // namespace math