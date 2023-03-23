#include "../lib/functions.hpp"
#include "../lib/io.hpp"
#include "../lib/polynomials.hpp"
#include "lib/differential-equations.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <utility>

using math::value_t;

value_t p(value_t x)
{
    return -(4.0 - x) / (5.0 - 2.0 * x);
}

value_t q(value_t x)
{
    return (1.0 - x) / 2.0;
}

value_t r(value_t x)
{
    return std::log(x + 3.0) / 2.0;
}

value_t f(value_t x)
{
    return 1.0 + x / 3.0;
}

static auto right_bound_conditions = std::make_pair<value_t, value_t>(1, 0);
static auto left_bound_conditions = std::make_pair<value_t, value_t>(1, 0);

int main(int, char **)
{
    std::cout << std::setprecision(10);
    /*
       Solve -(4.0 - x) / (5.0 - 2.0 * x) * y'' + (1.0 - x) / 2.0 y' + ln(x + 3) / 2 * y=1 + x/3, y(-1)=0, y(1)=0
       using Runge-Kutta method from -1 to 1 with step 0.2
    */

    int n = 10;

    auto galerkin_solution = diff::secondOrder::solveUsing::galerkin(
        p,
        q,
        r,
        f,
        math::polynomials::jacobi::getWeightedBasis(1, 1, n),
        math::polynomials::jacobi::getWeightedBasisDerivatives(1, 1, n),
        math::polynomials::jacobi::getWeightedBasisSecondDerivatives(1, 1, n),
        left_bound_conditions,
        right_bound_conditions
    );
    io::printFunction(galerkin_solution, "u", -1, 1, 0.2);

    auto collocation_chebyshev_solution = diff::secondOrder::solveUsing::collocation(
        p,
        q,
        r,
        f,
        math::polynomials::jacobi::getWeightedBasis(1, 1, n),
        math::polynomials::jacobi::getWeightedBasisDerivatives(1, 1, n),
        math::polynomials::jacobi::getWeightedBasisSecondDerivatives(1, 1, n),
        math::polynomials::chebyshev::getRoots(n),
        left_bound_conditions,
        right_bound_conditions
    );
    io::printFunction(collocation_chebyshev_solution, "u", -1, 1, 0.2);

    std::vector<value_t> equidistant_points;
    for (int i = 0; i < n; i++) {
        equidistant_points.push_back(-1.0 + 2.0 / (value_t)n * (i + 0.5));
    }

    auto collocation_equidistant_solution = diff::secondOrder::solveUsing::collocation(
        p,
        q,
        r,
        f,
        math::polynomials::jacobi::getWeightedBasis(1, 1, n),
        math::polynomials::jacobi::getWeightedBasisDerivatives(1, 1, n),
        math::polynomials::jacobi::getWeightedBasisSecondDerivatives(1, 1, n),
        equidistant_points,
        left_bound_conditions,
        right_bound_conditions
    );
    io::printFunction(collocation_equidistant_solution, "u", -1, 1, 0.2);
}
