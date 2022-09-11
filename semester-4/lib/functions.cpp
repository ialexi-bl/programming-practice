#include "functions.hpp"
#include "roots.hpp"
#include <array>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <map>
#include <numbers>

namespace functions
{
    Polynomial::Polynomial(const std::vector<long double> &coefs) : m_coefs(coefs)
    {
    }

    long double Polynomial::operator[](int k) const
    {
        return m_coefs[k];
    }
    long double Polynomial::operator()(long double x) const
    {
        long double result = 0;
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

    long double Polynomial::getDerivativeAbsMax(int order, long double a, long double b) const
    {
        Polynomial derivative = differentiate();
        for (int i = 1; i < order; i++) derivative = derivative.differentiate();

        std::vector<long double> roots =
            roots::findAllRootsUsing::tangents(derivative.getEvaluator(), a, b, 1e-8, static_cast<int>((b - a) / 0.1));
        return getAbsMaxOnInterval(roots, getEvaluator(), a, b);
    }
    Func Polynomial::getEvaluator() const
    {
        return [=](long double x) {
            return (*this)(x);
        };
    }
    ExtrCalculator Polynomial::getDerivativeAbsMaxCalculator(int order) const
    {
        return [=](long double a, long double b) {
            return getDerivativeAbsMax(order, a, b);
        };
    }
    Polynomial Polynomial::differentiate() const
    {
        std::vector<long double> new_coefs;
        new_coefs.insert(new_coefs.begin(), m_coefs.size() == 0 ? m_coefs.begin() : m_coefs.begin() + 1, m_coefs.end());
        for (int i = 0, s = new_coefs.size(); i < s; i++) {
            new_coefs[i] *= (i + 1);
        }
        return new_coefs;
    }

    Polynomial Polynomial::integrate(long double c) const
    {
        std::vector<long double> new_coefs {c};
        new_coefs.insert(new_coefs.begin() + 1, m_coefs.begin(), m_coefs.end());

        for (int i = 1, s = new_coefs.size(); i < s; i++) {
            new_coefs[i] /= i;
        }
        return new_coefs;
    }
    long double Polynomial::integrate(long double a, long double b) const
    {
        const Polynomial antiderivative = integrate();
        return antiderivative(b) - antiderivative(a);
    }

    std::ostream &operator<<(std::ostream &stream, const Polynomial &polynomial)
    {
        return stream << static_cast<std::string>(polynomial);
    }

    std::vector<Func> legendrePolynomials {[](long double x) -> long double {
                                               return 1;
                                           },
                                           [](long double x) -> long double {
                                               return x;
                                           }};
    void initLegendrePolynomial(int n)
    {
        assert(n < 0 || "Cannot construct Legendre polynomial for n < 0");

        if (n >= legendrePolynomials.size()) {
            initLegendrePolynomial(n - 1);
            legendrePolynomials.push_back([n](long double x) -> long double {
                return (2.0 * static_cast<long double>(n) - 1.0) / n * legendrePolynomials[n - 1](x) * x -
                       (static_cast<long double>(n) - 1.0) / n * legendrePolynomials[n - 2](x);
            });
        }
    }

    Func getLegendrePolynomial(int n)
    {
        initLegendrePolynomial(n);
        return legendrePolynomials[n];
    }

    std::vector<long double> getLegendreRoots(int n)
    {
        initLegendrePolynomial(n);
        return roots::findAllRootsUsing::tangents(legendrePolynomials[n], -1, 1, 1e-12, 1000);
    }

    long double getGaussCoefficient(int n, long double a, long double b, long double xk)
    {
        assert(n > 0 || "Unable to compute gauss coefficients for n <= 0");
        initLegendrePolynomial(n - 1);

        long double q = (b - a) / 2;
        long double p = legendrePolynomials[n - 1](xk);
        return q * 2.0 * (1.0 - xk * xk) / (n * n * p * p);
    }

    long double getGaussCoefficient(int n, long double xk)
    {
        return getGaussCoefficient(n, -1, 1, xk);
    }

    std::vector<std::pair<long double, long double>> getGaussCoefficients(int n, long double a, long double b)
    {
        std::vector<long double> roots = getLegendreRoots(n);
        std::vector<std::pair<long double, long double>> result;
        result.reserve(roots.size());

        long double q = (b - a) / 2;
        for (long double root : roots) {
            result.push_back({a + q * (root + 1), getGaussCoefficient(n, a, b, root)});
        }

        return result;
    }

    std::vector<std::pair<long double, long double>> getGaussCoefficients(int n)
    {
        return getGaussCoefficients(n, -1, 1);
    }

    std::vector<long double> calculateDerivativeValues(Func f, long double a, long double b, int m)
    {
        assert(m >= 2 || "Cannot calculate derivative for less than 3 points");

        std::vector<long double> result;
        result.reserve(m + 1);

        long double h = (b - a) / m;
        result.push_back((-3 * f(a) + 4 * f(a + h) - f(a + 2 * h)) / (2 * h));

        for (int i = 1; i < m; i++) {
            long double x = a + i * h;
            result.push_back((f(x + h) - f(x - h)) / (2 * h));
        }

        result.push_back((3 * f(b) - 4 * f(b - h) + f(b - 2 * h)) / (2 * h));

        return result;
    }
    std::vector<long double> calculateDerivativeValuesPrecise(Func f, long double a, long double b, int m)
    {
        assert(m >= 3 || "Cannot calculate precise derivative for less than 4 points");

        std::vector<long double> result;
        result.reserve(m + 1);

        long double h = (b - a) / m;
        result.push_back((-11 * f(a) + 18 * f(a + h) - 9 * f(a + 2 * h) + 2 * f(a + 3 * h)) / (6 * h));
        result.push_back((f(a + 2 * h) - f(a)) / (2 * h));

        for (int i = 2; i < m - 1; i++) {
            long double x = a + i * h;
            result.push_back((f(x - 2 * h) - 8 * f(x - h) + 8 * f(x + h) - f(x + 2 * h)) / (12 * h));
        }

        result.push_back((f(b) - f(b - 2 * h)) / (2 * h));
        result.push_back((11 * f(b) - 18 * f(b - h) + 9 * f(b - 2 * h) - 2 * f(b - 3 * h)) / (6 * h));

        return result;
    }
    std::vector<long double> calculateSecondDerivativeValues(Func f, long double a, long double b, int m)
    {
        std::vector<long double> result;
        result.reserve(m + 1);
        result.push_back(std::nanl("impossible"));

        long double h = (b - a) / m;
        for (int i = 1; i < m; i++) {
            long double x = a + i * h;
            result.push_back((f(x - h) - 2 * f(x) + f(x + h)) / (h * h));
        }

        result.push_back(std::nanl("impossible"));
        return result;
    }

    long double getAbsMaxOnInterval(std::vector<long double> maxima, Func f, long double a, long double b)
    {
        long double result = std::max(std::abs(f(a)), std::abs(f(b)));

        for (long double maximum : maxima) {
            if (a <= maximum && maximum <= b) {
                result = std::max(result, std::abs(f(maximum)));
            }
        }
        return result;
    }

    std::vector<std::pair<long double, long double>> getMelerCoefficients(int n)
    {
        return getMelerCoefficients(n, -1, 1);
    }
    std::vector<std::pair<long double, long double>> getMelerCoefficients(int n, long double a, long double b)
    {
        long double q = (b - a) / 2;

        std::vector<std::pair<long double, long double>> result;
        result.reserve(n);

        long double c = q * std::numbers::pi / n;
        for (int k = 1; k <= n; k++) {
            long double xk = std::cos((2 * k - 1) * std::numbers::pi / (2 * n));
            result.push_back({a + q * (xk + 1), c});
        }

        return result;
    }
    long double calculateMelerIntegral(Func f, int n, long double a, long double b)
    {
        std::vector<std::pair<long double, long double>> coefs = getMelerCoefficients(n, a, b);

        long double result = 0;
        for (auto &&it : coefs) {
            result += it.second * f(it.first);
        }

        return result;
    }

    long double calculateMoment(Func w, long double a, long double b, int k)
    {
        return calculateIntegralUsing::compound::middleRect(
            [w, k](long double x) -> long double {
                return w(x) * std::pow(x, k);
            },
            a, b, 10000);
    }
    std::vector<long double> calculateMoments(Func w, long double a, long double b, int n)
    {
        std::vector<long double> result;
        result.reserve(n);
        for (int k = 0; k < n; k++) result.push_back(calculateMoment(w, a, b, k));
        return result;
    }

    static std::pair<long double, long double>
    solveSecondOrderLinearSystem(const std::array<long double, 4> &coefs,
                                 const std::pair<long double, long double> &constantTerms)
    {
        long double D = coefs[0] * coefs[3] - coefs[1] * coefs[2];

        if (D == 0) {
            throw std::runtime_error("Attempted to solve a linear system with zero determinant");
        }

        long double D1 = constantTerms.first * coefs[3] - coefs[1] * constantTerms.second;
        long double D2 = coefs[0] * constantTerms.second - constantTerms.first * coefs[2];

        return {D1 / D, D2 / D};
    }

    Polynomial getOrthogonalPolynomial(Func w, long double a, long double b)
    {
        const std::vector<long double> moments = calculateMoments(w, a, b, 4);
        const std::pair<long double, long double> coefs =
            solveSecondOrderLinearSystem({moments[1], moments[0], moments[2], moments[1]}, {-moments[2], -moments[3]});

        return {{coefs.second, coefs.first, 1}};
    }

    std::pair<long double, long double> getOrthogonalPolynomialRoots(Func w, long double a, long double b)
    {
        const Polynomial polynomial = getOrthogonalPolynomial(w, a, b);
        long double D = std::sqrt(polynomial[1] * polynomial[1] - 4 * polynomial[0] * polynomial[2]);
        return {(-polynomial[1] - D) / 2, (-polynomial[1] + D) / 2};
    }

    std::vector<std::pair<long double, long double>> getArbitraryWeightCoefs(Func w, long double a, long double b)
    {
        const std::pair<long double, long double> roots = getOrthogonalPolynomialRoots(w, a, b);
        const std::pair<long double, long double> coefs = solveSecondOrderLinearSystem(
            {1, 1, roots.first, roots.second}, {calculateMoment(w, a, b, 0), calculateMoment(w, a, b, 1)});
        return {{roots.first, coefs.first}, {roots.second, coefs.second}};
    }

    long double calculateArbitrareWeightIntegral(Func f, Func w, long double a, long double b)
    {
        std::vector<std::pair<long double, long double>> coefs = getArbitraryWeightCoefs(w, a, b);

        long double result = 0;
        for (int i = 0, s = coefs.size(); i < s; i++) {
            result += coefs[i].second * f(coefs[i].first);
        }
        return result;
    }

    namespace calculateIntegralUsing
    {
        long double leftRect(Func f, long double a, long double b)
        {
            return (b - a) * f(a);
        }
        long double middleRect(Func f, long double a, long double b)
        {
            long double mid = a + (b - a) / 2;
            return (b - a) * f(mid);
        }
        long double rightRect(Func f, long double a, long double b)
        {
            return (b - a) * f(b);
        }
        long double trapezoid(Func f, long double a, long double b)
        {
            return (b - a) * (f(a) + f(b)) / 2;
        }
        long double simpson(Func f, long double a, long double b)
        {
            long double mid = a + (b - a) / 2;
            return (b - a) * (f(a) + 4 * f(mid) + f(b)) / 6;
        }
        long double threeEights(Func f, long double a, long double b)
        {
            long double h = (b - a) / 3;
            return (b - a) * (f(a) + 3 * f(a + h) + 3 * f(b - h) + f(b)) / 8;
        }
        long double gauss(Func f, int n, long double a, long double b)
        {
            const std::vector<std::pair<long double, long double>> coefs = getGaussCoefficients(n, a, b);
            long double result = 0;
            for (auto &&it : coefs) {
                result += it.second * f(it.first);
            }

            return result;
        }
        long double meler(Func f, int n, long double a, long double b)
        {
            std::vector<std::pair<long double, long double>> coefs = getMelerCoefficients(n, a, b);

            long double result = 0;
            for (auto &&it : coefs) {
                result += it.second * f(it.first);
            }

            return result;
        }

        namespace compound
        {
            static long double applyCompoundFormula(Func f, long double a, long double b, int m,
                                                    std::function<long double(Func, long double, long double)> applyFormula)
            {
                long double result = 0, h = (b - a) / m;
                for (int i = 1; i <= m; i++) {
                    result += applyFormula(f, a + h * (i - 1), a + h * i);
                }
                return result;
            }

            long double leftRect(Func f, long double a, long double b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::leftRect);
            }
            long double middleRect(Func f, long double a, long double b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::middleRect);
            }
            long double rightRect(Func f, long double a, long double b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::rightRect);
            }
            long double trapezoid(Func f, long double a, long double b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::trapezoid);
            }
            long double simpson(Func f, long double a, long double b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::simpson);
            }
            long double threeEights(Func f, long double a, long double b, int m)
            {
                return applyCompoundFormula(f, a, b, m, calculateIntegralUsing::threeEights);
            }
            long double gauss(Func f, int n, long double a, long double b, int m, Integrator integrate)
            {
                const std::vector<std::pair<long double, long double>> coefs = getGaussCoefficients(n);
                return applyCompoundFormula(f, a, b, m, [integrate, &coefs](Func f, long double c, long double d) -> long double {
                    long double result = 0;
                    long double q = (d - c) / 2;
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
            long double leftRect(ExtrCalculator getExtr, long double a, long double b, int m)
            {
                return (1.0 / 2) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 1);
            }
            long double middleRect(ExtrCalculator getExtr, long double a, long double b, int m)
            {
                return (1.0 / 2) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 2);
            }
            long double rightRect(ExtrCalculator getExtr, long double a, long double b, int m)
            {
                return (1.0 / 2) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 1);
            }
            long double trapezoid(ExtrCalculator getExtr, long double a, long double b, int m)
            {
                return (1.0 / 12) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 2);
            }
            long double simpson(ExtrCalculator getExtr, long double a, long double b, int m)
            {
                return (1.0 / 24) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 4);
            }
            long double threeEights(ExtrCalculator getExtr, long double a, long double b, int m)
            {
                return (1.0 / 2880) * getExtr(a, b) * (b - a) * std::pow((b - a) / m, 4);
            }
        } // namespace compound
    }     // namespace getTheoreticalErrorOf

} // namespace functions