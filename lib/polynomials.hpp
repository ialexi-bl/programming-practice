#pragma once
#include "./math.hpp"
#include <functional>
#include <vector>

namespace polynomials
{
    using math::simple_function_t;
    using math::value_t;

    namespace jacobi
    {
        value_t evaluate(int a, int b, int n, value_t x);
        simple_function_t get(int a, int b, int n);

        value_t evaluateDerivative(int a, int b, int n, value_t x);
        simple_function_t getDerivative(int a, int b, int n);

        value_t evaluateWeighted(int a, int b, int n, value_t x);
        simple_function_t getWeighted(int a, int b, int n);

        value_t evaluateWeightedDerivative(int a, int b, int n, value_t x);
        simple_function_t getWeightedDerivative(int a, int b, int n);
        value_t evaluateWeightedSecondDerivative(int a, int b, int n, value_t x);
        simple_function_t getWeightedSecondDerivative(int a, int b, int n);

        std::vector<simple_function_t> getWeightedBasis(int a, int b, int n);
        std::vector<simple_function_t> getWeightedBasisDerivatives(int a, int b, int n);
        std::vector<simple_function_t> getWeightedBasisSecondDerivatives(int a, int b, int n);
    } // namespace jacobi

    namespace chebyshev
    {
        std::vector<value_t> getRoots(int n);
    }
} // namespace polynomials