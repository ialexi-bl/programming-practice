#pragma once
#include "../../lib/math.hpp"
#include <utility>

namespace diff
{
    using math::simple_function_t;
    using math::value_t;

    namespace secondOrder
    {
        namespace solveUsing
        {
            simple_function_t galerkin(
                simple_function_t p,
                simple_function_t q,
                simple_function_t r,
                simple_function_t f,
                std::vector<simple_function_t> basis,
                std::vector<simple_function_t> derivatives,
                std::vector<simple_function_t> secondDerivatives,
                std::pair<value_t, value_t> left_bound_conditions,
                std::pair<value_t, value_t> right_bound_conditions
            );

            simple_function_t collocation(
                simple_function_t p,
                simple_function_t q,
                simple_function_t r,
                simple_function_t f,
                std::vector<simple_function_t> basis,
                std::vector<simple_function_t> derivatives,
                std::vector<simple_function_t> secondDerivatives,
                std::vector<value_t> nodes,
                std::pair<value_t, value_t> left_bound_conditions,
                std::pair<value_t, value_t> right_bound_conditions
            );
        } // namespace solveUsing
    }     // namespace secondOrder
} // namespace diff