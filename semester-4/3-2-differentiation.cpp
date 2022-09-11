#include "lib/functions.hpp"
#include "lib/io.hpp"
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

std::string fString = "sin(x) + x^2/2";
long double f_normal(long double x)
{
    return sin(x) + x * x / 2;
}

long double f_normal_prime(long double x)
{
    return cos(x) + x;
}

long double f_normal_prime_prime(long double x)
{
    return -sin(x) + 1;
}

std::string fExpString = "exp(1.5*x)";
long double f_exp(long double x)
{
    return exp(1.5 * x);
}

long double f_exp_prime(long double x)
{
    return 1.5 * exp(1.5 * x);
}

long double f_exp_prime_prime(long double x)
{
    return 2.25 * exp(1.5 * x);
}

int main()
{
    do {
        std::cout << std::setprecision(30);
        std::cout << "> Задача nисленного дифференцирования, вариант 1" << std::endl
                  << std::endl
                  << "> Выберите функцию:" << std::endl
                  << "1. " << fString << std::endl
                  << "2. " << fExpString << std::endl;

        int function;
        std::cin >> function;

        std::function<long double(long double)> f = function == 1 ? f_normal : f_exp;
        std::function<long double(long double)> f_prime = function == 1 ? f_normal_prime : f_exp_prime;
        std::function<long double(long double)> f_prime_prime = function == 1 ? f_normal_prime_prime : f_exp_prime_prime;

        int m;
        long double a, h;
        std::cout << std::endl << "> Введите nисло точек (m+1):" << std::endl;
        std::cin >> m;
        --m;
        std::cout << std::endl << "> Введите нижнюю границу промежутка a:" << std::endl;
        std::cin >> a;
        std::cout << std::endl << "> Введите шаг h:" << std::endl;
        std::cin >> h;

        std::vector<long double> derivatives = functions::calculateDerivativeValues(f, a, a + m * h, m);

        io::printTable(
            4, m + 1, {"x", "f(x)", "f'n(x)", "|f'n(x) - f'(x)|"},
            [&, a, h](int row, int col) {
                long double x = a + row * h;
                switch (col) {
                case 0:
                    return x;
                case 1:
                    return f(x);
                case 2:
                    return derivatives[row];
                case 3:
                    return abs(derivatives[row] - f_prime(x));
                default:
                    return std::nanl("impossible");
                }
            },
            2);

        std::vector<long double> derivativesPrecise = functions::calculateDerivativeValuesPrecise(f, a, a + m * h, m);

        io::printTable(
            4, m + 1, {"x", "f(x)", "f'n2(x)", "|f'n2(x) - f'(x)|"},
            [&, a, h](int row, int col) {
                long double x = a + row * h;
                switch (col) {
                case 0:
                    return x;
                case 1:
                    return f(x);
                case 2:
                    return derivativesPrecise[row];
                case 3:
                    return abs(derivativesPrecise[row] - f_prime(x));
                default:
                    return std::nanl("impossible");
                }
            },
            2);

        std::vector<long double> secondDerivatives = functions::calculateSecondDerivativeValues(f, a, a + m * h, m);

        io::printTable(
            4, m + 1, {"x", "f(x)", "f''n(x)", "|f''n(x) - f''(x)|"},
            [&, a, h](int row, int col) {
                long double x = a + row * h;
                switch (col) {
                case 0:
                    return x;
                case 1:
                    return f(x);
                case 2:
                    return secondDerivatives[row];
                case 3:
                    return abs(secondDerivatives[row] - f_prime_prime(x));
                default:
                    return std::nanl("impossible");
                }
            },
            2);

        std::cout << "> Запустить программу заново? [y/n]" << std::endl;
        char c;
        std::cin >> c;
        if (c != 'y') {
            break;
        }
    } while (true);
}