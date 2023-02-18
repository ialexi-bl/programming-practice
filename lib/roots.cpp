#include "roots.hpp"
#include <iostream>

namespace roots
{

    std::vector<std::pair<long double, long double>> getIntervalsWithRoots(std::function<long double(long double)> f,
                                                                           long double a, long double b, int N)
    {
        long double h = (b - a) / N;

        std::vector<std::pair<long double, long double>> intervals;
        for (long double x = a; x < b; x += h) {
            long double v1 = f(x), v2 = f(x + h);
            if (v1 * v2 < 0 || v2 == 0 || (x == a && v1 == 0)) {
                intervals.push_back({x, x + h});
            }
        }

        return intervals;
    }

    namespace findRootUsing
    {

        long double bisection(std::function<long double(long double)> f, long double a, long double b, long double epsilon,
                              bool printDebugInfo)
        {
            if (printDebugInfo) std::cout << "- Метод бисекции:" << std::endl;

            int step = 0;
            long double c;
            do {
                ++step;
                c = a + (b - a) / 2;
                if (f(a) * f(c) <= 0) {
                    b = c;
                } else {
                    a = c;
                }
            } while (b - a > 2 * epsilon);

            if (printDebugInfo)
                std::cout << "    " << step << " шагов" << std::endl
                          << "    Длина последнего отрезка: " << b - a << std::endl
                          << "    Решение: " << c << std::endl
                          << "    Модуль невязки: " << abs(f(c)) << std::endl;

            return c;
        }

        long double newton(std::function<long double(long double)> f, long double (*f_prime)(long double),
                           long double (*f_prime_prime)(long double), long double a, long double b, long double epsilon,
                           bool printDebugInfo)
        {
            if (printDebugInfo) std::cout << "- Метод Ньютона" << std::endl;

            int step = 0;
            long double prev = b, x0 = a;

            if (f(x0) * f_prime_prime(x0) <= 0) {
                prev = a;
                x0 = b;
            }

            if (printDebugInfo) std::cout << "    x0 = " << x0 << std::endl;

            while (abs(prev - x0) > epsilon && step < 1e9) {
                ++step;
                prev = x0;
                x0 = x0 - f(x0) / f_prime(x0);
                if (printDebugInfo)
                    std::cout << "    |x_" << step << " - x_" << step - 1 << "| = " << abs(prev - x0) << std::endl;
            }

            if (printDebugInfo)
                std::cout << "    " << step << " шагов" << std::endl
                          << "    Решение: " << x0 << std::endl
                          << "    Модуль невязки: " << abs(f(x0)) << std::endl;
            return x0;
        }

        long double newtonModified(std::function<long double(long double)> f, long double (*f_prime)(long double),
                                   long double (*f_prime_prime)(long double), long double a, long double b, long double epsilon,
                                   bool printDebugInfo)
        {
            if (printDebugInfo) std::cout << "- Модифицированный метод Ньютона" << std::endl;
            int step = 0;

            long double prev = b, x0 = a;
            if (f(x0) * f_prime_prime(x0) <= 0) {
                prev = a;
                x0 = b;
            }

            if (printDebugInfo) std::cout << "    x0 = " << x0 << std::endl;

            long double y = f_prime(x0);
            while (abs(prev - x0) > epsilon && step < 1e9) {
                ++step;
                prev = x0;
                x0 = x0 - f(x0) / y;
            }

            if (printDebugInfo)
                std::cout << "    " << step << " шагов" << std::endl
                          << "    Решение: " << x0 << std::endl
                          << "    Модуль невязки: " << abs(f(x0)) << std::endl;
            return x0;
        }

        long double tangents(std::function<long double(long double)> f, long double a, long double b, long double epsilon,
                             bool printDebugInfo)
        {
            if (printDebugInfo) std::cout << "- Метод секущих от " << a << " до " << b << std::endl;
            int step = 0;

            long double h = (b - a) / 3;
            long double x0 = a + h, x1 = b - h, t;
            if (printDebugInfo) std::cout << "    x0 = " << x0 << ", x1 = " << x1 << std::endl;

            while (abs(x1 - x0) > epsilon && step < 1e9) {
                ++step;
                t = x0;
                x0 = x0 - (x0 - x1) * f(x0) / (f(x0) - f(x1));
                x1 = t;
            }

            if (printDebugInfo)
                std::cout << "    " << step << " шагов" << std::endl
                          << "    Решение: " << x0 << std::endl
                          << "    Модуль невязки: " << abs(f(x0)) << std::endl;
            return x0;
        }

    } // namespace findRootUsing

    namespace findAllRootsUsing
    {
        std::vector<long double> tangents(std::function<long double(long double)> f, long double a, long double b,
                                          long double epsilon, int N)
        {

            std::vector<std::pair<long double, long double>> intervals = getIntervalsWithRoots(f, a, b, N);

            std::vector<long double> roots;
            roots.reserve(intervals.size());

            for (auto &&interval : intervals) {
                roots.push_back(findRootUsing::tangents(f, interval.first, interval.second, epsilon));
            }

            return roots;
        }
    } // namespace findAllRootsUsing

} // namespace roots