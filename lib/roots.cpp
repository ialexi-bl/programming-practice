#include "roots.hpp"
#include <cmath>
#include <iostream>

namespace math
{

    std::vector<std::pair<value_t, value_t>> getIntervalsWithRoots(simple_function_t f, value_t a, value_t b, int N)
    {
        value_t h = (b - a) / N;

        std::vector<std::pair<value_t, value_t>> intervals;
        for (value_t x = a; x < b; x += h) {
            value_t v1 = f(x), v2 = f(x + h);
            if (v1 * v2 < 0 || v2 == 0 || (x == a && v1 == 0)) {
                intervals.push_back({x, x + h});
            }
        }

        return intervals;
    }

    namespace findRootUsing
    {

        value_t bisection(simple_function_t f, value_t a, value_t b, value_t epsilon, bool print_debug_info)
        {
            if (print_debug_info) std::cout << "- Метод бисекции:" << std::endl;

            int step = 0;
            value_t c;
            do {
                ++step;
                c = a + (b - a) / 2;
                if (f(a) * f(c) <= 0) {
                    b = c;
                } else {
                    a = c;
                }
            } while (b - a > 2 * epsilon);

            if (print_debug_info)
                std::cout << "    " << step << " шагов" << std::endl
                          << "    Длина последнего отрезка: " << b - a << std::endl
                          << "    Решение: " << c << std::endl
                          << "    Модуль невязки: " << std::abs(f(c)) << std::endl;

            return c;
        }

        value_t newton(
            simple_function_t f,
            value_t (*f_prime)(value_t),
            value_t (*f_prime_prime)(value_t),
            value_t a,
            value_t b,
            value_t epsilon,
            bool print_debug_info
        )
        {
            if (print_debug_info) std::cout << "- Метод Ньютона" << std::endl;

            int step = 0;
            value_t prev = b, x0 = a;

            if (f(x0) * f_prime_prime(x0) <= 0) {
                prev = a;
                x0 = b;
            }

            if (print_debug_info) std::cout << "    x0 = " << x0 << std::endl;

            while (std::abs(prev - x0) > epsilon && step < 1e9) {
                ++step;
                prev = x0;
                x0 = x0 - f(x0) / f_prime(x0);
                if (print_debug_info)
                    std::cout << "    |x_" << step << " - x_" << step - 1 << "| = " << std::abs(prev - x0) << std::endl;
            }

            if (print_debug_info)
                std::cout << "    " << step << " шагов" << std::endl
                          << "    Решение: " << x0 << std::endl
                          << "    Модуль невязки: " << std::abs(f(x0)) << std::endl;
            return x0;
        }

        value_t newtonModified(
            simple_function_t f,
            value_t (*f_prime)(value_t),
            value_t (*f_prime_prime)(value_t),
            value_t a,
            value_t b,
            value_t epsilon,
            bool print_debug_info
        )
        {
            if (print_debug_info) std::cout << "- Модифицированный метод Ньютона" << std::endl;
            int step = 0;

            value_t prev = b, x0 = a;
            if (f(x0) * f_prime_prime(x0) <= 0) {
                prev = a;
                x0 = b;
            }

            if (print_debug_info) std::cout << "    x0 = " << x0 << std::endl;

            value_t y = f_prime(x0);
            while (std::abs(prev - x0) > epsilon && step < 1e9) {
                ++step;
                prev = x0;
                x0 = x0 - f(x0) / y;
            }

            if (print_debug_info)
                std::cout << "    " << step << " шагов" << std::endl
                          << "    Решение: " << x0 << std::endl
                          << "    Модуль невязки: " << std::abs(f(x0)) << std::endl;
            return x0;
        }

        value_t tangents(simple_function_t f, value_t a, value_t b, value_t epsilon, bool print_debug_info)
        {
            if (print_debug_info) std::cout << "- Метод секущих от " << a << " до " << b << std::endl;
            int step = 0;

            value_t h = (b - a) / 3;
            value_t x0 = a + h, x1 = b - h, t;
            if (print_debug_info) std::cout << "    x0 = " << x0 << ", x1 = " << x1 << std::endl;

            while (std::abs(x1 - x0) > epsilon && step < 1e9) {
                ++step;
                t = x0;
                x0 = x0 - (x0 - x1) * f(x0) / (f(x0) - f(x1));
                x1 = t;
            }

            if (print_debug_info)
                std::cout << "    " << step << " шагов" << std::endl
                          << "    Решение: " << x0 << std::endl
                          << "    Модуль невязки: " << std::abs(f(x0)) << std::endl;
            return x0;
        }

    } // namespace findRootUsing

    namespace findAllRootsUsing
    {
        std::vector<value_t> tangents(simple_function_t f, value_t a, value_t b, value_t epsilon, int N)
        {

            std::vector<std::pair<value_t, value_t>> intervals = getIntervalsWithRoots(f, a, b, N);

            std::vector<value_t> roots;
            roots.reserve(intervals.size());

            for (auto &&interval : intervals) {
                roots.push_back(findRootUsing::tangents(f, interval.first, interval.second, epsilon));
            }

            return roots;
        }
    } // namespace findAllRootsUsing

} // namespace math