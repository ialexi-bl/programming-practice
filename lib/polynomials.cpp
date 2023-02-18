#include "./polynomials.hpp"
#include <cassert>
#include <iostream>
#include <map>
#include <stdexcept>
#include <vector>

namespace polynomials
{
    using polynomial_list = std::vector<simple_function_t>;
    static std::map<int, polynomial_list> jacobiPolynomials;

    static void prepareJacobiPolynomial(int a, int b, int n, polynomial_list &polynomials)
    {
        assert(polynomials.size() >= 2 || "Polynomials list was not properly initialized");

        if (n < (int)polynomials.size()) {
            return;
        }

        prepareJacobiPolynomial(a, b, n - 1, polynomials);
        polynomials.push_back([a, n, &polynomials](value_t x) -> value_t {
            value_t k = a, m = n;
            return (m + k) * (2 * m + 2.0 * k - 1.0) / ((m + 2.0 * k) * m) * (x * polynomials[n - 1](x)) -
                   (m + k) * (m + k - 1.0) / ((m + 2.0 * k) * m) * polynomials[n - 2](x);
        });
    }
    static void prepareJacobiPolynomial(int a, int b, int n)
    {
        assert(n >= 0 || "Cannot construct Jacobi polynomial for n < 0");
        assert(a == b || "Jacobi polynomials for a!=b are not supported");

        if (jacobiPolynomials.find(a) == jacobiPolynomials.end()) {
            jacobiPolynomials[a] = {
                [](value_t x) {
                    return 1.0;
                },
                [a](value_t x) {
                    return (a + 1) * x;
                }};
        }
        prepareJacobiPolynomial(a, b, n, jacobiPolynomials[a]);
    }

    namespace jacobi
    {
        value_t evaluate(int a, int b, int n, value_t x)
        {
            prepareJacobiPolynomial(a, b, n);
            return jacobiPolynomials[a][n](x);
        }
        simple_function_t get(int a, int b, int n)
        {
            prepareJacobiPolynomial(a, b, n);
            return jacobiPolynomials[a][n];
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
            std::vector<simple_function_t> secondDerivatives;
            for (int i = 0; i < n; i++) {
                secondDerivatives.push_back(getWeightedSecondDerivative(a, b, i));
            }
            return secondDerivatives;
        }
    } // namespace jacobi

} // namespace polynomials