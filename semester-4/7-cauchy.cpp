#include "lib/differential-equations.hpp"
#include "lib/functions.hpp"
#include "lib/io.hpp"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <utility>

long double f(long double x, long double y)
{
    return -y + x / 2;
}

int x0 = 0;
namespace solution
{
    functions::Polynomial taylor {{1, -1, 0.75, -0.25, 0.0625}};

    long double evaluate(long double x)
    {
        return (x - 1 + 3 * std::exp(-x)) / 2;
    }
} // namespace solution

int main()
{
    std::cout << std::setprecision(15);

    int n = 10;
    long double h = 0.1;

    do {
        std::cout << "> Введите размер таблицы n:" << std::endl;
        std::cin >> n;

        std::cout << std::endl << "> Введите шаг h:" << std::endl;
        std::cin >> h;

        std::cout << std::endl << "> Таблица точных значений и приближений с помощью ряда Тейлора" << std::endl;
        io::printTable(4, n + 3, {"x", "y(x)", "P_y(x)", "|y(x)-P_y(x)|"}, [=](int row, int col) -> long double {
            long double x = x0 + (row - 2) * h;
            switch (col) {
            case 0:
                return x;
            case 1:
                return solution::evaluate(x);
            case 2:
                return solution::taylor(x);
            case 3:
                return std::abs(solution::evaluate(x) - solution::taylor(x));
            default:
                return std::nanl("impossible");
            }
        });

        std::cout << std::endl << "> Таблица приближений с помощью метода Адамса 4 порядка" << std::endl;

        std::vector<long double> bigTableStart(5);
        std::generate(bigTableStart.begin(), bigTableStart.end(), [=, m = -2]() mutable -> long double {
            return solution::taylor(x0 + h * (m++));
        });

        std::vector<long double> adams = diff::continueTableUsing::adams(bigTableStart, f, x0 + 2 * h, h, n + 3);
        io::printTable(3, n + 3, {"x", "y'(x)", "|y(x)-y'(x)|"}, [=, &adams](int row, int col) -> long double {
            long double x = x0 + h * (row - 2);
            switch (col) {
            case 0:
                return x;
            case 1:
                return adams[row];
            case 2:
                return std::abs(solution::evaluate(x) - adams[row]);
            default:
                return std::nanl("impossible");
            }
        });

        std::cout << std::endl << "> Таблица приближений с помощью метода Рунге-Кутты 4 порядка" << std::endl;

        std::vector<long double> tableStart {1};

        std::vector<long double> rungeKutta = diff::continueTableUsing::rungeKutta(tableStart, f, x0, h, n);
        io::printTable(3, n + 1, {"x", "y'(x)", "|y(x)-y'(x)|"}, [=, &rungeKutta](int row, int col) -> long double {
            long double x = x0 + h * (row);
            switch (col) {
            case 0:
                return x;
            case 1:
                return rungeKutta[row];
            case 2:
                return std::abs(solution::evaluate(x) - rungeKutta[row]);
            default:
                return std::nanl("impossible");
            }
        });

        std::cout << std::endl << "> Таблица приближений с помощью метода Эйлера без модификаций" << std::endl;

        std::vector<long double> euler = diff::continueTableUsing::euler(tableStart, f, x0, h, n);
        io::printTable(3, n + 1, {"x", "y'(x)", "|y(x)-y'(x)|"}, [=, &euler](int row, int col) -> long double {
            long double x = x0 + h * (row);
            switch (col) {
            case 0:
                return x;
            case 1:
                return euler[row];
            case 2:
                return std::abs(solution::evaluate(x) - euler[row]);
            default:
                return std::nanl("impossible");
            }
        });

        std::cout << std::endl << "> Таблица приближений с помощью метода Эйлера 1" << std::endl;

        std::vector<long double> euler1 = diff::continueTableUsing::euler1(tableStart, f, x0, h, n);
        io::printTable(3, n + 1, {"x", "y'(x)", "|y(x)-y'(x)|"}, [=, &euler1](int row, int col) -> long double {
            long double x = x0 + h * (row);
            switch (col) {
            case 0:
                return x;
            case 1:
                return euler1[row];
            case 2:
                return std::abs(solution::evaluate(x) - euler1[row]);
            default:
                return std::nanl("impossible");
            }
        });

        std::cout << std::endl << "> Таблица приближений с помощью метода Эйлера 2" << std::endl;

        std::vector<long double> euler2 = diff::continueTableUsing::euler2(tableStart, f, x0, h, n);
        io::printTable(3, n + 1, {"x", "y'(x)", "|y(x)-y'(x)|"}, [=, &euler2](int row, int col) -> long double {
            long double x = x0 + h * (row);
            switch (col) {
            case 0:
                return x;
            case 1:
                return euler2[row];
            case 2:
                return std::abs(solution::evaluate(x) - euler2[row]);
            default:
                return std::nanl("impossible");
            }
        });

        long double realValue = solution::evaluate(x0 + n * h);
        std::vector<std::pair<std::string, long double>> errors {
            {"Разложение по Тейлору: ", std::abs(realValue - solution::taylor(x0 + n * h))},
            {"Метод Адамса:          ", std::abs(realValue - adams[n + 2])},
            {"Метод Рунге-Кутты:     ", std::abs(realValue - rungeKutta[n + 2])},
            {"Метод Эйлера:          ", std::abs(realValue - euler[n + 2])},
            {"Метод Эйлера 1:        ", std::abs(realValue - euler1[n + 2])},
            {"Метод Эйлера 2:        ", std::abs(realValue - euler2[n + 2])}};
        std::sort(errors.begin(), errors.end(),
                  [](const std::pair<std::string, long double> &a, const std::pair<std::string, long double> &b) {
                      return a.second < b.second;
                  });

        std::cout << std::endl << "> Погрешность последнего значения" << std::endl;
        for (auto &&it : errors) {
            std::cout << "  " << it.first << it.second << std::endl;
        }
        std::cout << std::endl;

        std::cout << "> Введите новые значения n и h через пробел или перевод строки, чтобы продолжить, или 0, чтобы выйти"
                  << std::endl;

        std::cin >> n;
        if (n <= 0) {
            break;
        }
        std::cin >> h;
    } while (true);
}