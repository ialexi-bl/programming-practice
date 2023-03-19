#include "./polynomials.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <map>
#include <numbers>
#include <stdexcept>
#include <vector>

namespace polynomials
{
    using polynomial_list = std::vector<simple_function_t>;

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
                value_t Pi_x = (i + k) * (2 * i + 2.0 * k - 1.0) / ((i + 2.0 * k) * i) * (x * P1_x) -
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
            std::vector<simple_function_t> secondDerivatives;
            for (int i = 0; i < n; i++) {
                secondDerivatives.push_back(getWeightedSecondDerivative(a, b, i));
            }
            return secondDerivatives;
        }
    } // namespace jacobi

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
} // namespace polynomials