#pragma once
#include <array>
#include <functional>
#include <string>
#include <vector>

namespace functions
{
    using Func = std::function<long double(long double)>;
    using Integrator = std::function<long double(long double, long double)>;
    using ExtrCalculator = std::function<long double(long double, long double)>;

    class Polynomial
    {
      private:
        std::vector<long double> m_coefs;

      public:
        Polynomial() = default;
        Polynomial(const std::vector<long double> &coefs);

        long double operator[](int k) const;
        long double operator()(long double x) const;
        operator std::string() const;

        Func getEvaluator() const;

        long double getDerivativeAbsMax(int order, long double a, long double b) const;
        ExtrCalculator getDerivativeAbsMaxCalculator(int order) const;

        Polynomial differentiate() const;
        Polynomial integrate(long double c = 0) const;
        long double integrate(long double a, long double b) const;
    };

    std::ostream &operator<<(std::ostream &stream, const Polynomial &polynomial);

    extern std::vector<Func> legendrePolynomials;
    Func getLegendrePolynomial(int n);
    std::vector<long double> getLegendreRoots(int n);

    long double getGaussCoefficient(int n, long double xk);
    long double getGaussCoefficient(int n, long double a, long double b, long double xk);
    std::vector<std::pair<long double, long double>> getGaussCoefficients(int n);
    std::vector<std::pair<long double, long double>> getGaussCoefficients(int n, long double a, long double b);

    std::vector<long double> calculateDerivativeValues(Func f, long double a, long double b, int m);
    std::vector<long double> calculateDerivativeValuesPrecise(Func f, long double a, long double b, int m);
    std::vector<long double> calculateSecondDerivativeValues(Func f, long double a, long double b, int m);

    long double getAbsMaxOnInterval(std::vector<long double> maxima, Func f, long double a, long double b);

    std::vector<std::pair<long double, long double>> getMelerCoefficients(int n);
    std::vector<std::pair<long double, long double>> getMelerCoefficients(int n, long double a, long double b);
    long double calculateMelerIntegral(Func f, int n, long double a, long double b);

    long double calculateMoment(Func f, long double a, long double b, int k);
    std::vector<long double> calculateMoments(Func f, long double a, long double b, int n);

    Polynomial getOrthogonalPolynomial(Func w, long double a, long double b);
    std::pair<long double, long double> getOrthogonalPolynomialRoots(Func w, long double a, long double b);
    std::vector<std::pair<long double, long double>> getArbitraryWeightCoefs(Func w, long double a, long double b);

    long double calculateArbitrareWeightIntegral(Func f, Func w, long double a, long double b);

    namespace calculateIntegralUsing
    {
        long double leftRect(Func f, long double a, long double b);
        long double middleRect(Func f, long double a, long double b);
        long double rightRect(Func f, long double a, long double b);
        long double trapezoid(Func f, long double a, long double b);
        long double simpson(Func f, long double a, long double b);
        long double threeEights(Func f, long double a, long double b);
        long double gauss(Func f, int n, long double a, long double b);
        long double meler(Func f, int n, long double a, long double b);

        namespace compound
        {
            long double leftRect(Func f, long double a, long double b, int m);
            long double middleRect(Func f, long double a, long double b, int m);
            long double rightRect(Func f, long double a, long double b, int m);
            long double trapezoid(Func f, long double a, long double b, int m);
            long double simpson(Func f, long double a, long double b, int m);
            long double threeEights(Func f, long double a, long double b, int m);
            long double gauss(Func f, int n, long double a, long double b, int m, Integrator integrate);
        } // namespace compound
    }     // namespace calculateIntegralUsing

    namespace getTheoreticalErrorOf
    {
        namespace compound
        {
            long double leftRect(ExtrCalculator getExtr, long double a, long double b, int m);
            long double middleRect(ExtrCalculator getExtr, long double a, long double b, int m);
            long double rightRect(ExtrCalculator getExtr, long double a, long double b, int m);
            long double trapezoid(ExtrCalculator getExtr, long double a, long double b, int m);
            long double simpson(ExtrCalculator getExtr, long double a, long double b, int m);
            long double threeEights(ExtrCalculator getExtr, long double a, long double b, int m);
        } // namespace compound
    }     // namespace getTheoreticalErrorOf

} // namespace functions