#pragma once
#include "math.hpp"
#include <array>
#include <functional>
#include <string>
#include <vector>

namespace functions
{
    using math::value_t;

    using Func = math::simple_function_t;
    using Integrator = std::function<value_t(value_t, value_t)>;
    using ExtrCalculator = std::function<value_t(value_t, value_t)>;

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

        Func getEvaluator() const;

        value_t getDerivativeAbsMax(int order, value_t a, value_t b) const;
        ExtrCalculator getDerivativeAbsMaxCalculator(int order) const;

        Polynomial differentiate() const;
        Polynomial integrate(value_t c = 0) const;
        value_t integrate(value_t a, value_t b) const;
    };

    std::ostream &operator<<(std::ostream &stream, const Polynomial &polynomial);

    extern std::vector<Func> legendrePolynomials;
    Func getLegendrePolynomial(int n);
    std::vector<value_t> getLegendreRoots(int n);

    value_t getGaussCoefficient(int n, value_t xk);
    value_t getGaussCoefficient(int n, value_t a, value_t b, value_t xk);
    std::vector<std::pair<value_t, value_t>> getGaussCoefficients(int n);
    std::vector<std::pair<value_t, value_t>> getGaussCoefficients(int n, value_t a, value_t b);

    std::vector<value_t> calculateDerivativeValues(Func f, value_t a, value_t b, int m);
    std::vector<value_t> calculateDerivativeValuesPrecise(Func f, value_t a, value_t b, int m);
    std::vector<value_t> calculateSecondDerivativeValues(Func f, value_t a, value_t b, int m);

    value_t getAbsMaxOnInterval(std::vector<value_t> maxima, Func f, value_t a, value_t b);

    std::vector<std::pair<value_t, value_t>> getMelerCoefficients(int n);
    std::vector<std::pair<value_t, value_t>> getMelerCoefficients(int n, value_t a, value_t b);
    value_t calculateMelerIntegral(Func f, int n, value_t a, value_t b);

    value_t calculateMoment(Func f, value_t a, value_t b, int k);
    std::vector<value_t> calculateMoments(Func f, value_t a, value_t b, int n);

    Polynomial getOrthogonalPolynomial(Func w, value_t a, value_t b);
    std::pair<value_t, value_t> getOrthogonalPolynomialRoots(Func w, value_t a, value_t b);
    std::vector<std::pair<value_t, value_t>> getArbitraryWeightCoefs(Func w, value_t a, value_t b);

    value_t calculateArbitrareWeightIntegral(Func f, Func w, value_t a, value_t b);

    namespace calculateIntegralUsing
    {
        value_t leftRect(Func f, value_t a, value_t b);
        value_t middleRect(Func f, value_t a, value_t b);
        value_t rightRect(Func f, value_t a, value_t b);
        value_t trapezoid(Func f, value_t a, value_t b);
        value_t simpson(Func f, value_t a, value_t b);
        value_t threeEights(Func f, value_t a, value_t b);
        value_t gauss(Func f, int n, value_t a, value_t b);
        value_t meler(Func f, int n, value_t a, value_t b);

        namespace compound
        {
            value_t leftRect(Func f, value_t a, value_t b, int m);
            value_t middleRect(Func f, value_t a, value_t b, int m);
            value_t rightRect(Func f, value_t a, value_t b, int m);
            value_t trapezoid(Func f, value_t a, value_t b, int m);
            value_t simpson(Func f, value_t a, value_t b, int m);
            value_t threeEights(Func f, value_t a, value_t b, int m);
            value_t gauss(Func f, int n, value_t a, value_t b, int m, Integrator integrate);
        } // namespace compound
    }     // namespace calculateIntegralUsing

    namespace getTheoreticalErrorOf
    {
        namespace compound
        {
            value_t leftRect(ExtrCalculator getExtr, value_t a, value_t b, int m);
            value_t middleRect(ExtrCalculator getExtr, value_t a, value_t b, int m);
            value_t rightRect(ExtrCalculator getExtr, value_t a, value_t b, int m);
            value_t trapezoid(ExtrCalculator getExtr, value_t a, value_t b, int m);
            value_t simpson(ExtrCalculator getExtr, value_t a, value_t b, int m);
            value_t threeEights(ExtrCalculator getExtr, value_t a, value_t b, int m);
        } // namespace compound
    }     // namespace getTheoreticalErrorOf

} // namespace functions