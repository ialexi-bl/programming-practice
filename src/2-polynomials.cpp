#include "lib/io.hpp"
#include "lib/polynomials.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

std::string fString = "sin(x) - x²/2";
long double f(long double x)
{
    return sin(x) - x * x / 2;
    // return x * x;
}

int main()
{
    std::cout << std::setprecision(25);
    std::cout << "> Задача алгебраического интерполирования, вариант 14"
              << std::endl
              << "> Функция: f(x) = " << fString << std::endl
              << std::endl;

    int m = 5;
    std::cout << "- Введите число значений m в таблице:" << std::endl;
    // std::cin >> m;
    m--;

    long double a = 1, b = 8;
    std::cout
        << "- Введите границы интервала a и b через пробел или перевод строки:"
        << std::endl;
    // std::cin >> a >> b;
    std::cout << std::endl;

    polynomials::InterpolationTable table =
        polynomials::getInterpolationTable(f, m, a, b);

    std::cout << "> Таблица значений для интерполяции:" << std::endl;
    io::printTable(table, "x", "f(x)", 2);
    std::cout << std::endl;

    long double x = 2;
    std::cout << "- Введите точку интерполяции:" << std::endl;
    // std::cin >> x;

    int n = 4;
    while (true) {
        std::cout
            << "- Введите степень n интерполяционного многочлена не больше "
            << m << ":" << std::endl;
        // std::cin >> n;

        if (n <= m) {
            break;
        }

        std::cout << "! Недопустимая степень" << std::endl;
    }
    std::cout << std::endl;

    polynomials::sortInterpolationTable(table, x);
    long double realValue = f(x);
    long double lagrangeValue =
        polynomials::interpolateUsing::lagrange(table, n, x);
    long double newtonValue =
        polynomials::interpolateUsing::newton(table, n, x);

    std::cout << "> Результат интерполяции по Лагранжу: " << lagrangeValue
              << std::endl
              << "> Фактическая погрешность: " << abs(realValue - lagrangeValue)
              << std::endl
              << std::endl
              << "> Результат интерполяции по Ньютону: " << newtonValue
              << std::endl
              << "> Фактическая погрешность: " << abs(realValue - newtonValue)
              << std::endl;
}