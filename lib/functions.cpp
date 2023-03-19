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

namespace functions
{
    Polynomial::Polynomial(const std::vector<value_t> &coefs) : m_coefs(coefs)
    {
    }

    value_t Polynomial::operator[](int k) const
    {
        return m_coefs[k];
    }
    value_t Polynomial::operator()(value_t x) const
    {
        value_t result = 0;
        for (auto coef = m_coefs.rbegin(); coef != m_coefs.rend(); coef++) {
            result *= x;
            result += *coef;
        }
        return result;
    }
    Polynomial::operator std::string() const
    {
        std::string str = "";
        for (int i = 0, s = m_coefs.size(); i < s; i++) {
            if (m_coefs[i] == 0) {
                continue;
            }
            if (str.size() > 0) {
                str += " + ";
            }
            str += std::to_string(m_coefs[i]);
            if (i) {
                str += 'x';
                if (i > 1) {
                    str += '^';
                    str += std::to_string(i);
                }
            }
        }
        return str;
    }

    value_t Polynomial::getDerivativeAbsMax(int order, value_t a, value_t b) const
    {
        Polynomial derivative = differentiate();
        for (int i = 1; i < order; i++) derivative = derivative.differentiate();

        std::vector<value_t> roots =
            roots::findAllRootsUsing::tangents(derivative.getEvaluator(), a, b, 1e-8, static_cast<int>((b - a) / 0.1));
        return getAbsMaxOnInterval(roots, getEvaluator(), a, b);
    }
    Func Polynomial::getEvaluator() const
    {
        return [this](value_t x) {
            return (*this)(x);
        };
    }
    ExtrCalculator Polynomial::getDerivativeAbsMaxCalculator(int order) const
    {
        return [this, order](value_t a, value_t b) {
            return getDerivativeAbsMax(order, a, b);
        };
    }
    Polynomial Polynomial::differentiate() const
    {
        std::vector<value_t> new_coefs;
        new_coefs.insert(new_coefs.begin(), m_coefs.size() == 0 ? m_coefs.begin() : m_coefs.begin() + 1, m_coefs.end());
        for (int i = 0, s = new_coefs.size(); i < s; i++) {
            new_coefs[i] *= (i + 1);
        }
        return new_coefs;
    }

    Polynomial Polynomial::integrate(value_t c) const
    {
        std::vector<value_t> new_coefs {c};
        new_coefs.insert(new_coefs.begin() + 1, m_coefs.begin(), m_coefs.end());

        for (int i = 1, s = new_coefs.size(); i < s; i++) {
            new_coefs[i] /= i;
        }
        return new_coefs;
    }
    value_t Polynomial::integrate(value_t a, value_t b) const
    {
        const Polynomial antiderivative = integrate();
        return antiderivative(b) - antiderivative(a);
    }

    std::ostream &operator<<(std::ostream &stream, const Polynomial &polynomial)
    {
        return stream << static_cast<std::string>(polynomial);
    }

    value_t evaluateLegendrePolynomial(int n, value_t x)
    {
        assert(n >= 0 || "Cannot construct Legendre polynomial for n < 0");

        value_t P0_x = 1.0;
        value_t P1_x = x;

        if (n == 0) {
            return P0_x;
        }
        if (n == 1) {
            return P1_x;
        }

        for (value_t i = 2; i <= n; i++) {
            value_t Pi_x = (2.0 * i - 1.0) / i * P1_x * x - (i - 1.0) / i * P0_x;

            P0_x = P1_x;
            P1_x = Pi_x;
        }
        return P1_x;
    }

    Func getLegendrePolynomial(int n)
    {
        return [n](value_t x) {
            return evaluateLegendrePolynomial(n, x);
        };
    }

    std::vector<value_t> getLegendreRoots(int n)
    {
        return roots::findAllRootsUsing::tangents(getLegendrePolynomial(n), -1, 1, 1e-15, 1e5);
    }

    value_t getGaussCoefficient(int n, value_t a, value_t b, value_t xk)
    {
        assert(n > 0 || "Unable to compute gauss coefficients for n <= 0");

        value_t q = (b - a) / 2;
        value_t p = evaluateLegendrePolynomial(n - 1, xk);
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

    std::vector<value_t> calculateDerivativeValues(Func f, value_t a, value_t b, int m)
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
    std::vector<value_t> calculateDerivativeValuesPrecise(Func f, value_t a, value_t b, int m)
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
    std::vector<value_t> calculateSecondDerivativeValues(Func f, value_t a, value_t b, int m)
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

    value_t getAbsMaxOnInterval(std::vector<value_t> maxima, Func f, value_t a, value_t b)
    {
        value_t result = std::max(std::abs(f(a)), std::abs(f(b)));

        for (value_t maximum : maxima) {
            if (a <= maximum && maximum <= b) {
                result = std::max(result, std::abs(f(maximum)));
            }
        }
        return result;
    }

    std::vector<std::pair<value_t, value_t>> getMelerCoefficients(int n)
    {
        return getMelerCoefficients(n, -1, 1);
    }
    std::vector<std::pair<value_t, value_t>> getMelerCoefficients(int n, value_t a, value_t b)
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
    value_t calculateMelerIntegral(Func f, int n, value_t a, value_t b)
    {
        std::vector<std::pair<value_t, value_t>> coefs = getMelerCoefficients(n, a, b);

        value_t result = 0;
        for (auto &&it : coefs) {
            result += it.second * f(it.first);
        }

        return result;
    }

    value_t calculateMoment(Func w, value_t a, value_t b, int k)
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
    std::vector<value_t> calculateMoments(Func w, value_t a, value_t b, int n)
    {
        std::vector<value_t> result;
        result.reserve(n);
        for (int k = 0; k < n; k++) result.push_back(calculateMoment(w, a, b, k));
        return result;
    }

    static std::pair<value_t, value_t>
    solveSecondOrderLinearSystem(const std::array<value_t, 4> &coefs, const std::pair<value_t, value_t> &constantTerms)
    {
        value_t D = coefs[0] * coefs[3] - coefs[1] * coefs[2];

        if (D == 0) {
            throw std::runtime_error("Attempted to solve a linear system with zero determinant");
        }

        value_t D1 = constantTerms.first * coefs[3] - coefs[1] * constantTerms.second;
        value_t D2 = coefs[0] * constantTerms.second - constantTerms.first * coefs[2];

        return {D1 / D, D2 / D};
    }

    Polynomial getOrthogonalPolynomial(Func w, value_t a, value_t b)
    {
        const std::vector<value_t> moments = calculateMoments(w, a, b, 4);
        const std::pair<value_t, value_t> coefs =
            solveSecondOrderLinearSystem({moments[1], moments[0], moments[2], moments[1]}, {-moments[2], -moments[3]});

        return {{coefs.second, coefs.first, 1}};
    }

    std::pair<value_t, value_t> getOrthogonalPolynomialRoots(Func w, value_t a, value_t b)
    {
        const Polynomial polynomial = getOrthogonalPolynomial(w, a, b);
        value_t D = std::sqrt(polynomial[1] * polynomial[1] - 4 * polynomial[0] * polynomial[2]);
        return {(-polynomial[1] - D) / 2, (-polynomial[1] + D) / 2};
    }

    std::vector<std::pair<value_t, value_t>> getArbitraryWeightCoefs(Func w, value_t a, value_t b)
    {
        const std::pair<value_t, value_t> roots = getOrthogonalPolynomialRoots(w, a, b);
        const std::pair<value_t, value_t> coefs = solveSecondOrderLinearSystem(
            {1, 1, roots.first, roots.second},
            {calculateMoment(w, a, b, 0), calculateMoment(w, a, b, 1)}
        );
        return {{roots.first, coefs.first}, {roots.second, coefs.second}};
    }

    value_t calculateArbitrareWeightIntegral(Func f, Func w, value_t a, value_t b)
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
        value_t leftRect(Func f, value_t a, value_t b)
        {
            return (b - a) * f(a);
        }
        value_t middleRect(Func f, value_t a, value_t b)
        {
            value_t mid = a + (b - a) / 2;
            return (b - a) * f(mid);
        }
        value_t rightRect(Func f, value_t a, value_t b)
        {
            return (b - a) * f(b);
        }
        value_t trapezoid(Func f, value_t a, value_t b)
        {
            return (b - a) * (f(a) + f(b)) / 2;
        }
        value_t simpson(Func f, value_t a, value_t b)
        {
            value_t mid = a + (b - a) / 2;
            return (b - a) * (f(a) + 4 * f(mid) + f(b)) / 6;
        }
        value_t threeEights(Func f, value_t a, value_t b)
        {
            value_t h = (b - a) / 3;
            return (b - a) * (f(a) + 3 * f(a + h) + 3 * f(b - h) + f(b)) / 8;
        }
        value_t gauss(Func f, int n, value_t a, value_t b)
        {
            const std::vector<std::pair<value_t, value_t>> coefs = getGaussCoefficients(n, a, b);
            value_t result = 0;
            for (auto &&it : coefs) {
                result += it.second * f(it.first);
            }

            return result;
        }
        value_t meler(Func f, int n, value_t a, value_t b)
        {
            std::vector<std::pair<value_t, value_t>> coefs = getMelerCoefficients(n, a, b);

            value_t result = 0;
            for (auto &&it : coefs) {
                result += it.second * f(it.first);
            }

            return result;
        }

        namespace compound
        {
            static value_t
            applyCompoundFormula(Func f, value_t a, value_t b, int m, std::function<value_t(Func, value_t, value_t)> applyFormula)
            {
                value_t result = 0, h = (b - a) / m;
                for (int i = 1; i <= m; i++) {
                    result += applyFormula(f, a + h * (i - 1), a + h * i);
                }
                return result;
            }

            value_t leftRect(Func f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::leftRect);
            }
            value_t middleRect(Func f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::middleRect);
            }
            value_t rightRect(Func f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::rightRect);
            }
            value_t trapezoid(Func f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::trapezoid);
            }
            value_t simpson(Func f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::simpson);
            }
            value_t threeEights(Func f, value_t a, value_t b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::threeEights);
            }
            value_t gauss(Func f, int n, value_t a, value_t b, int m, Integrator integrate)
            {
                const std::vector<std::pair<value_t, value_t>> coefs = getGaussCoefficients(n);
                return applyCompoundFormula(f, a, b, m, [integrate, &coefs](Func f, value_t c, value_t d) -> value_t {
                    value_t result = 0;
                    value_t q = (d - c) / 2;
                    for (auto &&it : coefs) {
                        result += q * it.second * f(c + (it.first + 1) * q);
                    }
                    // std::cout << std::setw(25) << c << " " << std::setw(25) << d << " " << std::setw(25) << result << " "
                    //           << std::abs(result - integrate(c, d)) << std::endl;

                    return result;
                });
            }
        } // namespace compound
    }     // namespace calculateIntegralUsing

    namespace getTheoreticalErrorOf
    {
        namespace compound
        {
            value_t leftRect(ExtrCalculator getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 2) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 1);
            }
            value_t middleRect(ExtrCalculator getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 2) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 2);
            }
            value_t rightRect(ExtrCalculator getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 2) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 1);
            }
            value_t trapezoid(ExtrCalculator getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 12) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 2);
            }
            value_t simpson(ExtrCalculator getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 24) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 4);
            }
            value_t threeEights(ExtrCalculator getExtr, value_t a, value_t b, int m)
            {
                return (1.0 / 2880) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 4);
            }
        } // namespace compound
    }     // namespace getTheoreticalErrorOf

} // namespace functions