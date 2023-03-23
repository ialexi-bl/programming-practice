#include "../lib/functions.hpp"
#include "../lib/io.hpp"
#include "../lib/roots.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

math::polynomials::Polynomial polynomials[] = {
    {{}},
    {{1.0l, 5.0l}},
    {{1.0l, 2.0l, -24.0l, 3.0l}},
    {{1.0l, -15.0l, 0.0l, -30.0l, 3.0l, -15.0l}},
    {{1.0l, -15.0l, 0.0l, -30.0l, 3.0l, -15.0l, 0.0l, -30.0l}},
    {{1.0l, -15.0l, 0.0l, -30.0l, 3.0l, -15.0l, 0.0l, -30.0l, 0.0, -108.0l}},
    {{1.0l, -15.0l, 0.0l, -30.0l, 3.0l, -15.0l, 0.0l, -30.0l, 0.0, -108.0l, 0.05l, 0.009l}},
    {{1.0l, -15.0l, 0.0l, -30.0l, 3.0l, -15.0l, 0.0l, -30.0l, 0.0, -108.0l, 0.05l, 0.009l, 0.0l, -0.4l}},
    {{1.0l, -15.0l, 0.0l, -30.0l, 3.0l, -15.0l, 0.0l, -30.0l, 0.0, -108.0l, 0.05l, 0.009l, 0.0l, -0.4l, 9.0l, -7.0l}},
    {{1.0l,
      -15.0l,
      0.0l,
      -30.0l,
      3.0l,
      -15.0l,
      0.0l,
      -30.0l,
      0.0,
      -108.0l,
      0.05l,
      0.009l,
      0.0l,
      -0.4l,
      9.0l,
      -7.0l,
      -9.0l,
      28.0l}}};

namespace f
{
    std::string str = "1/(1+x)√x";

    long double antiderivative(long double x)
    {
        return 2 * std::atan(std::sqrt(x));
    }

    long double integrate(long double a, long double b)
    {
        return antiderivative(b) - antiderivative(a);
    }

    long double evaluate(long double x)
    {
        return 1 / ((x + 1) * std::sqrt(x));
    }
} // namespace f

namespace f_meler
{
    std::string str = "cos(x)/(1+x²)";

    long double evaluate(long double x)
    {
        return std::cos(x) / (1 + x * x);
    }
} // namespace f_meler

int main()
{
    std::cout << std::setprecision(30);

    for (int n = 1; n <= 8; n++) {
        const std::vector<std::pair<long double, long double>> coefs = math::getGaussCoefficients(n, -1, 1);

        std::cout << "Коэффициенты формулы Гаусса для n = " << n << ":" << std::endl;
        io::printNodesAndCoefs(coefs);
        std::cout << std::endl;
    }

    std::cout << std::endl;

    for (int n : std::vector {3, 5, 8}) {
        const long double real = polynomials[n].integrate(-1, 1);
        const long double approximate = math::calculateIntegralUsing::gauss(polynomials[n], n, -1, 1);
        std::cout << "Проверка на многочлене степени " << (2 * n - 1) << std::endl
                  << polynomials[n] << std::endl
                  << "- Точное значение:       " << real << std::endl
                  << "- Приближённое значение: " << approximate << std::endl
                  << "- Погрешность:           " << std::abs(real - approximate) << std::endl
                  << std::endl;
    }

    std::cout << std::endl;

    {
        long double a = 0, b = 1;
        std::vector<int> n_values {4, 6, 8};
        do {
            std::cout << std::endl;
            for (int n : n_values) {
                const long double real = f::integrate(a, b);
                const long double approximate = math::calculateIntegralUsing::gauss(f::evaluate, n, a, b);
                const std::vector<std::pair<long double, long double>> coefs = math::getGaussCoefficients(n, a, b);

                std::cout << "Интеграл функции " << f::str << " от " << a << " до " << b << " с числом узлов " << n << ":"
                          << std::endl;

                std::cout << "- Узлы и коэффициенты:" << std::endl;
                io::printNodesAndCoefs(coefs);

                std::cout << "- Точное значение:       " << real << std::endl
                          << "- Приближённое значение: " << approximate << std::endl
                          << "- Погрешность:           " << std::abs(real - approximate) << std::endl
                          << std::endl;
            }

            std::cout << std::endl
                      << "> Введите новое значение n, чтобы посчитать интеграл ещё раз, или 0, чтобы продолжить:" << std::endl
                      << "$ ";
            int n;
            std::cin >> n;

            if (n < 1) {
                break;
            }
            n_values = {n};

            std::cout << "> Введите a и b через пробел или перевод строки:" << std::endl << "$ ";
            std::cin >> a >> b;
            std::cout << std::endl;
        } while (true);
    }

    std::cout << std::endl;

    {
        int n = 0;
        do {
            if (n == 0) {
                std::cout << "> Введите n для интегрирования функции " << f_meler::str << " по формуле Мелера:" << std::endl;
            } else {
                std::cout << "> Введите новое значение n или 0, чтобы выйти:" << std::endl;
            }

            std::cin >> n;

            if (n < 1) {
                break;
            }

            std::cout << std::endl;
            std::vector<std::pair<long double, long double>> melerCoefs = math::getMelerCoefficients(n);
            std::cout << "> Коэффициенты формулы Мелера:" << std::endl;
            io::printNodesAndCoefs(melerCoefs);

            std::cout << "> Приближённое значение интеграла: " << math::calculateIntegralUsing::meler(f_meler::evaluate, n, -1, 1)
                      << std::endl
                      << std::endl;
        } while (true);
    }

    std::cout << std::endl;
}