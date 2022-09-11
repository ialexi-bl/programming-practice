#include "lib/functions.hpp"
#include "lib/io.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

namespace w
{
    std::string str = "cos(x)";

    long double evaluate(long double x)
    {
        long double c = std::cos(x);
        return c;
    }
} // namespace w

namespace f
{
    std::string str = "sin(x)";

    long double evaluate(long double x)
    {
        return std::sin(x);
    }
} // namespace f

namespace weighted_f
{
    long double antiderivative(long double x)
    {
        long double cos = std::cos(x);
        return -cos * cos / 2;
    }

    long double integrate(long double a, long double b)
    {
        return antiderivative(b) - antiderivative(a);
    }

    long double evaluate(long double x)
    {
        return f::evaluate(x) * w::evaluate(x);
    }
} // namespace weighted_f

int main()
{
    std::cout << std::setprecision(100);

    std::cout << "Интегрирование функции f(x) = " << f::str << " с весом ρ(x) = " << w::str << std::endl << std::endl;

    long double a = 0, b = 1;
    std::cout << "> Введите пределы интегрирования a и b через пробел или перевод строки:" << std::endl;
    std::cin >> a >> b;
    std::cout << std::endl;

    {
        std::cout << "--- Интегрирование с помощью КФ типа Гаусса с двумя узлами ---" << std::endl;
        std::vector<long double> moments = functions::calculateMoments(w::evaluate, a, b, 4);
        std::cout << "> Моменты весовой функции: " << std::endl;
        io::printArrayOfVariables(moments, 0, "μ");

        functions::Polynomial orthogonalPolynomial = functions::getOrthogonalPolynomial(w::evaluate, a, b);
        std::cout << std::endl << "> Ортогональный многочлен: " << orthogonalPolynomial << std::endl;

        std::vector<std::pair<long double, long double>> coefs = functions::getArbitraryWeightCoefs(w::evaluate, a, b);
        std::cout << "> Узлы и коэффициенты формулы:" << std::endl;
        io::printNodesAndCoefs(coefs);

        long double integral = functions::calculateArbitrareWeightIntegral(f::evaluate, w::evaluate, a, b);
        std::cout << "> Приближённое значение интеграла: " << integral << std::endl
                  << "> Точное значение:                 " << weighted_f::integrate(a, b) << std::endl
                  << "> Погрешность:                     " << std::abs(integral - weighted_f::integrate(a, b)) << std::endl;
    }

    std::cout << std::endl << "--- Интегрирование с помощью КФ Гаусса ---" << std::endl;

    int n, m;
    std::cout << "> Введите число узлов N и число отрезков разбиения m через пробел или перевод строки:" << std::endl;
    std::cin >> n >> m;
    std::cout << std::endl;

    do {
        {
            const std::vector<std::pair<long double, long double>> coefs = functions::getGaussCoefficients(n, 0.05, 0.1);
            std::cout << "> Узлы и коэффициенты исходной КФ Гаусса:" << std::endl;
            io::printNodesAndCoefs(coefs);
            std::cout << std::endl;

            long double integral =
                functions::calculateIntegralUsing::compound::gauss(weighted_f::evaluate, n, a, b, m, weighted_f::integrate);
            std::cout << "> Приближённое значение интеграла: " << integral << std::endl
                      << "> Погрешность:                     " << std::abs(integral - weighted_f::integrate(a, b)) << std::endl;
        }

        std::cout << std::endl << "> Введите новые N и m через пробел или перевод строки или 0, чтобы выйти" << std::endl;
        std::cin >> n;
        if (n <= 0) break;

        std::cin >> m;
        std::cout << std::endl;
    } while (true);
}
