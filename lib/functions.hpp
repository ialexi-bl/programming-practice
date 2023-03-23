#pragma once
#include "math.hpp"
#include "polynomials.hpp"
#include <array>
#include <functional>
#include <string>
#include <vector>

namespace math
{
    using integrator_t = std::function<value_t(value_t, value_t)>;
    using extr_calculator_t = std::function<value_t(value_t, value_t)>;

    value_t getGaussCoefficient(int n, value_t xk);
    value_t getGaussCoefficient(int n, value_t a, value_t b, value_t xk);
    std::vector<std::pair<value_t, value_t>> getGaussCoefficients(int n);
    std::vector<std::pair<value_t, value_t>> getGaussCoefficients(int n, value_t a, value_t b);

    std::vector<value_t> calculateDerivativeValues(simple_function_t f, value_t a, value_t b, int m);
    std::vector<value_t> calculateDerivativeValuesPrecise(simple_function_t f, value_t a, value_t b, int m);
    std::vector<value_t> calculateSecondDerivativeValues(simple_function_t f, value_t a, value_t b, int m);

    value_t getAbsMaxOnInterval(std::vector<value_t> maxima, simple_function_t f, value_t a, value_t b);

    value_t calculateMoment(simple_function_t f, value_t a, value_t b, int k);
    std::vector<value_t> calculateMoments(simple_function_t f, value_t a, value_t b, int n);

    polynomials::Polynomial getOrthogonalPolynomial(simple_function_t w, value_t a, value_t b);
    std::vector<std::pair<value_t, value_t>> getArbitraryWeightCoefs(simple_function_t w, value_t a, value_t b);

    value_t calculateArbitraryWeightIntegral(simple_function_t f, simple_function_t w, value_t a, value_t b);

    namespace calculateIntegralUsing
    {
        value_t leftRect(simple_function_t f, value_t a, value_t b);
        value_t middleRect(simple_function_t f, value_t a, value_t b);
        value_t rightRect(simple_function_t f, value_t a, value_t b);
        value_t trapezoid(simple_function_t f, value_t a, value_t b);
        value_t simpson(simple_function_t f, value_t a, value_t b);
        value_t threeEights(simple_function_t f, value_t a, value_t b);
        value_t gauss(simple_function_t f, int n, value_t a, value_t b);
        value_t meler(simple_function_t f, int n, value_t a, value_t b);

        namespace compound
        {
            value_t leftRect(simple_function_t f, value_t a, value_t b, int m);
            value_t middleRect(simple_function_t f, value_t a, value_t b, int m);
            value_t rightRect(simple_function_t f, value_t a, value_t b, int m);
            value_t trapezoid(simple_function_t f, value_t a, value_t b, int m);
            value_t simpson(simple_function_t f, value_t a, value_t b, int m);
            value_t threeEights(simple_function_t f, value_t a, value_t b, int m);
            value_t gauss(simple_function_t f, int n, value_t a, value_t b, int m, integrator_t integrate);
        } // namespace compound
    }     // namespace calculateIntegralUsing

    namespace getTheoreticalErrorOf
    {
        namespace compound
        {
            value_t leftRect(extr_calculator_t getExtr, value_t a, value_t b, int m);
            value_t middleRect(extr_calculator_t getExtr, value_t a, value_t b, int m);
            value_t rightRect(extr_calculator_t getExtr, value_t a, value_t b, int m);
            value_t trapezoid(extr_calculator_t getExtr, value_t a, value_t b, int m);
            value_t simpson(extr_calculator_t getExtr, value_t a, value_t b, int m);
            value_t threeEights(extr_calculator_t getExtr, value_t a, value_t b, int m);
        } // namespace compound
    }     // namespace getTheoreticalErrorOf

} // namespace math