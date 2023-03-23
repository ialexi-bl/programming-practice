#include "differential-equations.hpp"
#include "../../lib/functions.hpp"
#include "../../lib/matrix.hpp"
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

namespace diff
{
    namespace secondOrder
    {
        namespace solveUsing
        {
            static value_t productL2(simple_function_t y, simple_function_t z)
            {
                return math::calculateIntegralUsing::compound::middleRect(
                    [=](value_t x) -> value_t {
                        return y(x) * z(x);
                    },
                    -1,
                    1,
                    100
                );
            }

            static simple_function_t linearCombination(std::vector<value_t> coefs, std::vector<simple_function_t> basis)
            {
                size_t n = basis.size();
                assert(n == coefs.size() || "Wrong amount of coefficientss");

                return [=](value_t x) -> value_t {
                    value_t result = 0;
                    for (size_t i = 0; i < n; i++) {
                        result += coefs[i] * basis[i](x);
                    }
                    return result;
                };
            }

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
            )
            {
                using namespace math;

                size_t n = basis.size();
                assert(n > 0 || "Basis must not be empty");
                assert(
                    (n == derivatives.size() && n == secondDerivatives.size()) ||
                    "Wrong amount of basis derivatives or second derivatives"
                );

                math::Matrix system(n, n + 1);
                for (size_t i = 0; i < n; i++) {
                    for (size_t j = 0; j < n; j++) {
                        system(i, j) = productL2(p * secondDerivatives[j] + q * derivatives[j] + r * basis[j], basis[i]);
                    }

                    system(i, n) = productL2(f, basis[i]);
                }

                auto coefs = math::solveLinearSystem(system);
                return linearCombination(coefs, basis);
            }

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
            )
            {
                using namespace math;

                size_t n = basis.size();
                assert(n > 0 || "Basis must not be empty");
                assert(
                    (n == derivatives.size() && n == secondDerivatives.size()) ||
                    "Wrong amount of basis derivatives or second derivatives"
                );
                assert(n == nodes.size() || "Wrong amount of nodes");

                math::Matrix system(n, n + 1);
                for (size_t i = 0; i < n; i++) {
                    value_t t = nodes[i];

                    for (size_t j = 0; j < n; j++) {
                        system(i, j) = p(t) * secondDerivatives[j](t) + q(t) * derivatives[j](t) + r(t) * basis[j](t);
                    }
                    system(i, n) = f(t);
                }

                auto coefs = math::solveLinearSystem(system);
                return linearCombination(coefs, basis);
            }
        } // namespace solveUsing
    }     // namespace secondOrder
} // namespace diff
