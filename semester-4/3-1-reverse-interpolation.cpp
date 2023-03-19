#include "../lib/io.hpp"
#include "../lib/roots.hpp"
#include "lib/polynomials.hpp"
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <string>

std::string fString = "sin(x) - x²/2";
long double f(long double x)
{
    // return exp(-x) - x * x / 2;
    return sin(x) - x * x / 2;
}

int main()
{
    std::cout << std::setprecision(30);
    std::cout << "> Задача алгебраического интерполирования, вариант 1" << std::endl
              << "> Функция: f(x) = " << fString << std::endl
              << std::endl;

    int m = 10;
    std::cout << "- Введите число значений m+1 в таблице:" << std::endl;
    std::cin >> m;
    m--;

    long double a = 0, b = 1;
    std::cout << "- Введите границы интервала a и b через пробел или перевод строки:" << std::endl;
    std::cin >> a >> b;

    long double epsilon = 1e-10;
    std::cout << "- Введите точность поиска корня:" << std::endl;
    std::cin >> epsilon;
    std::cout << std::endl;

    polynomials::InterpolationTable table = polynomials::getInterpolationTable(f, m, a, b);

    std::cout << "> Таблица узлов интерполяции:" << std::endl;
    io::printTable(table, "x", "f(x)", 2);
    std::cout << std::endl;

    polynomials::InterpolationTable invertedTable = polynomials::invertInterpolationTable(table);

    while (true) {
        long double F = 0.15;
        std::cout << "- Введите точку для поиска:" << std::endl;
        std::cin >> F;

        int n = 6;
        while (true) {
            std::cout << "- Введите степень n интерполяционного многочлена не больше " << m << ":" << std::endl;
            std::cin >> n;

            if (n <= m) {
                break;
            }

            std::cout << "! Недопустимая степень" << std::endl;
        }
        std::cout << std::endl;

        polynomials::sortInterpolationTable(invertedTable, F);

        std::cout << "> Отсортированная таблица интерполяции:" << std::endl;
        io::printTable(invertedTable, "x", "f(x)", 2, n + 1);

        long double firstValue = polynomials::interpolateUsing::lagrange(invertedTable, n, F);

        std::sort(
            table.begin(),
            table.end(),
            [F](const std::pair<long double, long double> &a, const std::pair<long double, long double> &b) {
                return std::abs(a.second - F) < std::abs(b.second - F);
            }
        );

        std::vector<long double> roots = roots::findAllRootsUsing::tangents(
            [&table, n, F](long double x) {
                return polynomials::interpolateUsing::lagrange(table, n, x) - F;
            },
            a,
            b,
            epsilon
        );

        std::cout << "==> Способ 1" << std::endl
                  << "      Ответ: x = " << firstValue << std::endl
                  << "      |f(x) - F| = " << std::abs(f(firstValue) - F) << std::endl
                  << "==> Способ 2" << std::endl
                  << "      Количество корней: " << roots.size() << std::endl;

        if (roots.size() > 0) {
            std::cout << "      Корни: " << std::endl;
            for (int i = 0; i < roots.size(); i++) {
                std::cout << "        " << i + 1 << ". x = " << roots[i] << std::endl
                          << "           Модуль невязки: |f(x) - F| = " << std::abs(f(roots[i]) - F) << std::endl;
            }
        }

        char c;
        std::cout << std::endl << "Указать другую точку интерполяции? [y/n]: ";
        std::cin >> c;
        std::cout << std::endl;

        if (c != 'Y' && c != 'y') {
            break;
        }
    }
}