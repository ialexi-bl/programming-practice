#pragma once
#include "math.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace math
{
    namespace polynomials
    {
        using extr_calculator_t = std::function<value_t(value_t, value_t)>;

        class Polynomial
        {
          private:
            std::vector<value_t> m_coefs;

          public:
            Polynomial() = default;
            Polynomial(const std::vector<value_t> &coefs);

            value_t operator[](int k) const;
            value_t operator()(value_t x) const;
            operator std::string() const;
            simple_function_t getEvaluator() const;

            value_t getDerivativeAbsMax(int order, value_t a, value_t b) const;
            extr_calculator_t getDerivativeAbsMaxCalculator(int order) const;

            Polynomial differentiate() const;
            Polynomial integrate(value_t c = 0) const;
            value_t integrate(value_t a, value_t b) const;
        };

        std::ostream &operator<<(std::ostream &stream, const Polynomial &polynomial);

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

        namespace legendre
        {
            value_t evaluate(int n, value_t x);
            simple_function_t get(int n);
        } // namespace legendre

        namespace chebyshev
        {
            std::vector<value_t> getRoots(int n);
        }
    } // namespace polynomials
} // namespace math