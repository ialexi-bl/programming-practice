#include "../lib/roots.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

std::string f_string = "(x-1)^2 - e^(-x)";
long double f(long double x)
{
    // return x - 3 * cos(1.04 * x) * cos(1.04 * x);
    return (x - 1) * (x - 1) - exp(-x);
}

long double f_prime(long double x)
{
    // return 1 + 3.12 * sin(2.08 * x);
    return 2 * (x - 1) + exp(-x);
}

long double f_prime_prime(long double x)
{
    // return 3.12 * 2.08 * cos(2.08 * x);
    return 2 - exp(-x);
}

int main()
{
    std::cout << "Приближённый поиск корней нечётной кратности" << std::endl
              << "Функция: f(x) = " << f_string << std::endl
              << std::endl;

    long double a = -1, b = 3;
    long double epsilon = 1e-8;

    std::cout << std::setprecision(15);
    std::cout << "Введите левую и правую границы промежутка через пробел или в "
                 "две строки:"
              << std::endl;
    std::cin >> a >> b;
    std::cout << "Введите требуемую точность: " << std::endl;
    std::cin >> epsilon;

    auto intervalsWithRoots = math::getIntervalsWithRoots(f, a, b);
    int intervalsCount = intervalsWithRoots.size();

    std::cout << std::endl << "Найдено " << intervalsCount << " отрезков с корнями";
    if (!intervalsCount) {
        std::cout << std::endl;
        return 0;
    }

    std::cout << ":" << std::endl;
    for (int i = 0; i < intervalsCount; i++) {
        std::cout << (i + 1) << ". [" << intervalsWithRoots[i].first << ", " << intervalsWithRoots[i].second << "]" << std::endl;
    }

    int i;
    std::cout << std::endl << "Введите номер отрезка:" << std::endl;
    std::cin >> i;
    --i;

    // for (int i = 0; i < intervalsCount; i++) {
    long double a0 = intervalsWithRoots[i].first;
    long double b0 = intervalsWithRoots[i].second;

    std::cout << std::endl << "Интервал " << (i + 1) << " – [" << a0 << ", " << b0 << "]" << std::endl;

    math::findRootUsing::bisection(f, a0, b0, epsilon, true);

    math::findRootUsing::newton(f, f_prime, f_prime_prime, a0, b0, epsilon, true);

    math::findRootUsing::newtonModified(f, f_prime, f_prime_prime, a0, b0, epsilon, true);

    math::findRootUsing::tangents(f, a0, b0, epsilon, true);
    // }
}