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
    return -1.0 / (x - 3.0);
}

value_t q(value_t x)
{
    return 1 + x / 2.0;
}

value_t r(value_t x)
{
    return -std::exp(x / 2.0);
}

value_t f(value_t x)
{
    return 2.0 - x;
}

static auto rightBoundConditions = std::make_pair<value_t, value_t>(1, 0);
static auto leftBoundConditions = std::make_pair<value_t, value_t>(1, 0);

int main(int, char **)
{
    std::cout << std::setprecision(10);
    // Solve -1/(x-3) * y'' + (1+x/2)y' - e^(x/2)y=2-x, y(-1)=0, y(1)=0 using Runge-Kutta method from -1 to 1 with step 0.2

    int n = 10;

    auto galerkinSolution = diff::secondOrder::solveUsing::galerkin(
        p,
        q,
        r,
        f,
        polynomials::jacobi::getWeightedBasis(1, 1, n),
        polynomials::jacobi::getWeightedBasisDerivatives(1, 1, n),
        polynomials::jacobi::getWeightedBasisSecondDerivatives(1, 1, n),
        leftBoundConditions,
        rightBoundConditions
    );
    io::printFunction(galerkinSolution, "u", -1, 1, 0.2);

    auto collocationChebyshevSolution = diff::secondOrder::solveUsing::collocation(
        p,
        q,
        r,
        f,
        polynomials::jacobi::getWeightedBasis(1, 1, n),
        polynomials::jacobi::getWeightedBasisDerivatives(1, 1, n),
        polynomials::jacobi::getWeightedBasisSecondDerivatives(1, 1, n),
        polynomials::chebyshev::getRoots(n),
        leftBoundConditions,
        rightBoundConditions
    );
    io::printFunction(collocationChebyshevSolution, "u", -1, 1, 0.2);

    std::vector<value_t> equidistantPoints;
    for (int i = 0; i < n; i++) {
        equidistantPoints.push_back(-1.0 + 2.0 / (value_t)n * (i + 0.5));
    }

    auto collocationEquidistantSolution = diff::secondOrder::solveUsing::collocation(
        p,
        q,
        r,
        f,
        polynomials::jacobi::getWeightedBasis(1, 1, n),
        polynomials::jacobi::getWeightedBasisDerivatives(1, 1, n),
        polynomials::jacobi::getWeightedBasisSecondDerivatives(1, 1, n),
        polynomials::chebyshev::getRoots(n),
        leftBoundConditions,
        rightBoundConditions
    );
    io::printFunction(collocationEquidistantSolution, "u", -1, 1, 0.2);
}
