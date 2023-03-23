#include "polynomials.hpp"
#include "roots.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <numbers>
#include <stdexcept>
#include <vector>

namespace math
{
    namespace polynomials
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

        static value_t getAbsMaxOnInterval(std::vector<value_t> maxima, simple_function_t f, value_t a, value_t b)
        {
            value_t result = std::max(std::abs(f(a)), std::abs(f(b)));

            for (value_t maximum : maxima) {
                if (a <= maximum && maximum <= b) {
                    result = std::max(result, std::abs(f(maximum)));
                }
            }
            return result;
        }

        value_t Polynomial::getDerivativeAbsMax(int order, value_t a, value_t b) const
        {
            Polynomial derivative = differentiate();
            for (int i = 1; i < order; i++) derivative = derivative.differentiate();

            std::vector<value_t> roots =
                math::findAllRootsUsing::tangents(derivative.getEvaluator(), a, b, 1e-8, static_cast<int>((b - a) / 0.1));
            return getAbsMaxOnInterval(roots, getEvaluator(), a, b);
        }
        simple_function_t Polynomial::getEvaluator() const
        {
            return [this](value_t x) {
                return (*this)(x);
            };
        }
        extr_calculator_t Polynomial::getDerivativeAbsMaxCalculator(int order) const
        {
            return [this, order](value_t a, value_t b) {
                return getDerivativeAbsMax(order, a, b);
            };
        }

        std::ostream &operator<<(std::ostream &stream, const Polynomial &polynomial)
        {
            return stream << static_cast<std::string>(polynomial);
        }

        namespace jacobi
        {
            value_t evaluate(int a, int b, int n, value_t x)
            {
                assert(n >= 0 || "Cannot construct Jacobi polynomial for n < 0");
                assert(a == b || "Jacobi polynomials for a!=b are not supported");
                value_t k = a;

                value_t P0_x = 1.0;
                value_t P1_x = (k + 1.0) * x;

                if (n == 0) {
                    return P0_x;
                }
                if (n == 1) {
                    return P1_x;
                }

                for (value_t i = 2; i <= n; i++) {
                    value_t Pi_x = (i + k) * (2.0 * i + 2.0 * k - 1.0) / ((i + 2.0 * k) * i) * (x * P1_x) -
                                   (i + k) * (i + k - 1.0) / ((i + 2.0 * k) * i) * P0_x;

                    P0_x = P1_x;
                    P1_x = Pi_x;
                }
                return P1_x;
            }
            simple_function_t get(int a, int b, int n)
            {
                return [=](value_t x) {
                    return evaluate(a, b, n, x);
                };
            }

            value_t evaluateDerivative(int a, int b, int n, value_t x)
            {
                if (n == 0) {
                    return 0;
                }

                return ((value_t)n + 2 * (value_t)a + 1) / 2.0 * evaluate(a + 1, b + 1, n - 1, x);
            }
            simple_function_t getDerivative(int a, int b, int n)
            {
                return [=](value_t x) {
                    return evaluateDerivative(a, b, n, x);
                };
            }

            value_t evaluateWeighted(int a, int b, int n, value_t x)
            {
                return (1 - x * x) * evaluate(a, b, n, x);
            }
            simple_function_t getWeighted(int a, int b, int n)
            {
                return [=](value_t x) {
                    return evaluateWeighted(a, b, n, x);
                };
            }

            value_t evaluateWeightedDerivative(int a, int b, int n, value_t x)
            {
                if (a == 0) {
                    return evaluateDerivative(a, b, n, x);
                }
                return -2.0 * ((value_t)n + 1.0) * evaluate(a - 1, b - 1, n + 1, x);
            }
            simple_function_t getWeightedDerivative(int a, int b, int n)
            {
                if (a == 0) {
                    return getDerivative(a, b, n);
                }
                return [=](value_t x) {
                    return evaluateWeightedDerivative(a, b, n, x);
                };
            }
            value_t evaluateWeightedSecondDerivative(int a, int b, int n, value_t x)
            {
                if (a == 0) {
                    throw std::logic_error("Not implemented");
                }
                return -2.0 * ((value_t)n + 1.0) * evaluateDerivative(a - 1, b - 1, n + 1, x);
            }
            simple_function_t getWeightedSecondDerivative(int a, int b, int n)
            {
                if (a == 0) {
                    throw std::logic_error("Not implemented");
                }
                return [=](value_t x) {
                    return evaluateWeightedSecondDerivative(a, b, n, x);
                };
            }

            std::vector<simple_function_t> getWeightedBasis(int a, int b, int n)
            {
                std::vector<simple_function_t> basis;
                for (int i = 0; i < n; i++) {
                    basis.push_back(jacobi::getWeighted(a, b, i));
                }
                return basis;
            }
            std::vector<simple_function_t> getWeightedBasisDerivatives(int a, int b, int n)
            {
                std::vector<simple_function_t> derivatives;
                for (int i = 0; i < n; i++) {
                    derivatives.push_back(getWeightedDerivative(a, b, i));
                }
                return derivatives;
            }
            std::vector<simple_function_t> getWeightedBasisSecondDerivatives(int a, int b, int n)
            {
                std::vector<simple_function_t> second_derivatives;
                for (int i = 0; i < n; i++) {
                    second_derivatives.push_back(getWeightedSecondDerivative(a, b, i));
                }
                return second_derivatives;
            }
        } // namespace jacobi

        namespace legendre
        {
            value_t evaluate(int n, value_t x)
            {
                return jacobi::evaluate(0, 0, n, x);
            }

            simple_function_t get(int n)
            {
                return [n](value_t x) {
                    return evaluate(n, x);
                };
            }
        } // namespace legendre

        namespace chebyshev
        {
            std::vector<value_t> getRoots(int n)
            {
                std::vector<value_t> result;
                for (value_t i = 1; i <= n; i++) {
                    result.push_back(std::cos((2.0 * i - 1) / (2 * (value_t)n) * std::numbers::pi));
                }
                return result;
            }
        } // namespace chebyshev
    }     // namespace polynomials
} // namespace math