#include "differential-equations.hpp"
#include "../../lib/functions.hpp"
#include "../../lib/matrix.hpp"
#include <cassert>
#include <iostream>
#include <utility>
#include <vector>

namespace diff
{

    namespace solveUsing
    {
        static value_t productL2(simple_function_t y, simple_function_t z)
        {
            return functions::calculateIntegralUsing::compound::middleRect(
                [=](value_t x) -> value_t {
                    return y(x) * z(x);
                },
                -1,
                1,
                50
            );
        }

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
        )
        {
            assert(basis.size() == derivatives.size() || "Lol");

            int n = basis.size();
            math::Matrix system(n, n + 1);

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    system(i, j) = productL2(
                        [&, i](value_t x) {
                            return p(x) * secondDerivatives[j](x) + q(x) * derivatives[j](x) + r(x) * basis[j](x);
                        },
                        basis[i]
                    );

                    std::cout << "(" << i << ", " << j << ") = " << system(i, j) << std::endl;
                }
            }
            for (int j = 0; j < n; j++) {
                system(j, n) = productL2(f, basis[j]);
            }

            std::cout << system << std::endl;

            auto coefs = math::solveLinearSystem(system);
            std::cout << coefs << std::endl;

            return [=](value_t x) -> value_t {
                value_t result = 0;
                for (int i = 0; i < n; i++) {
                    result += coefs[i] * basis[i](x);
                }
                return result;
            };
        }
    } // namespace solveUsing
} // namespace diff
