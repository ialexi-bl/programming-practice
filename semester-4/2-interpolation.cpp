#include "../lib/io.hpp"
#include "../lib/polynomials.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

std::string fString = "sin(x) - x²/2";
long double f(long double x)
{
    return sin(x) - x * x / 2;
}

int main()
{
    std::cout << std::setprecision(30);
    std::cout << "> Задача алгебраического интерполирования, вариант 1" << std::endl
              << "> Функция: f(x) = " << fString << std::endl
              << std::endl;

    int m;
    std::cout << "- Введите число значений m+1 в таблице:" << std::endl;
    std::cin >> m;
    m--;

    long double a, b;
    std::cout << "- Введите границы интервала a и b через пробел или перевод строки:" << std::endl;
    std::cin >> a >> b;
    std::cout << std::endl;

    polynomials::InterpolationTable table = polynomials::getInterpolationTable(f, m, a, b);

    std::cout << "> Таблица узлов интерполяции:" << std::endl;
    io::printTable(table, "x", "f(x)", 2);
    std::cout << std::endl;

    while (true) {
        long double x;
        std::cout << "- Введите точку интерполяции:" << std::endl;
        std::cin >> x;

        int n;
        while (true) {
            std::cout << "- Введите степень n интерполяционного многочлена не больше " << m << ":" << std::endl;
            std::cin >> n;

            if (n <= m) {
                break;
            }

            std::cout << "! Недопустимая степень" << std::endl;
        }
        std::cout << std::endl;

        polynomials::sortInterpolationTable(table, x);

        std::cout << "> Отсортированная таблица узлов интерполяции:" << std::endl;
        io::printTable(table, "x", "f(x)", 2, n + 1);

        long double realValue = f(x);
        long double lagrangeValue = polynomials::interpolateUsing::lagrange(table, n, x);
        long double newtonValue = polynomials::interpolateUsing::newton(table, n, x);

        char c;
        std::cout << "==> Результат интерполяции по Лагранжу: " << std::endl
                  << "      PL(x) = " << lagrangeValue << std::endl
                  << "==> Фактическая погрешность: " << std::endl
                  << "      |f(x) - PL(x)| = " << std::abs(realValue - lagrangeValue) << std::endl
                  << std::endl
                  << "==> Результат интерполяции по Ньютону: " << std::endl
                  << "      PN(x) = " << newtonValue << std::endl
                  << "==> Фактическая погрешность: " << std::endl
                  << "      |f(x) - PN(x)| = " << std::abs(realValue - newtonValue) << std::endl
                  << std::endl
                  << "Указать другую точку интерполяции? [y/n]: ";
        std::cin >> c;
        std::cout << std::endl;

        if (c != 'Y' && c != 'y') {
            break;
        }
    }
}