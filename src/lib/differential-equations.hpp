#pragma once
#include "../../lib/math.hpp"
#include <utility>

namespace diff
{
    using math::simple_function_t;
    using math::value_t;

    namespace solveUsing
    {
        simple_function_t galerkin(
            simple_function_t p,
            simple_function_t q,
            simple_function_t r,
            simple_function_t f,
            simple_function_t f_prime,
            simple_function_t f_prime_prime,
            std::vector<simple_function_t> basis,
            std::vector<simple_function_t> derivatives,
            std::vector<simple_function_t> secondDerivatives,
            std::pair<value_t, value_t> leftBoundConditions,
            std::pair<value_t, value_t> rightBoundConditions
        );
    }

} // namespace diff