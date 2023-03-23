#pragma once
#include "math.hpp"
#include <functional>
#include <utility>
#include <vector>

namespace math
{
    std::vector<std::pair<value_t, value_t>> getIntervalsWithRoots(simple_function_t f, value_t a, value_t b, int N = 10);

    namespace findRootUsing
    {

        value_t bisection(simple_function_t f, value_t a, value_t b, value_t epsilon, bool print_debug_info = false);
        value_t newton(
            simple_function_t f,
            value_t (*f_prime)(value_t),
            value_t (*f_prime_prime)(value_t),
            value_t a,
            value_t b,
            value_t epsilon,
            bool print_debug_info = false
        );
        value_t newtonModified(
            simple_function_t f,
            value_t (*f_prime)(value_t),
            value_t (*f_prime_prime)(value_t),
            value_t a,
            value_t b,
            value_t epsilon,
            bool print_debug_info = false
        );
        value_t tangents(simple_function_t f, value_t a, value_t b, value_t epsilon, bool print_debug_info = false);

    } // namespace findRootUsing

    namespace findAllRootsUsing
    {
        std::vector<value_t> tangents(simple_function_t f, value_t a, value_t b, value_t epsilon, int N = 10);
    }

} // namespace math