#include "../lib/functions.hpp"
#include "../lib/io.hpp"
#include "../lib/polynomials.hpp"
#include "lib/differential-equations.hpp"
#include <cmath>
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
value_t f_prime(value_t x)
{
    return -1.0;
}
value_t f_prime_prime(value_t x)
{
    return 0;
}

// Галеркин (1-x²)*Pi⁽¹¹⁾(x), i=0,1,...
// Коллокация

static auto rightBoundConditions = std::make_pair<value_t, value_t>(1, 0);
static auto leftBoundConditions = std::make_pair<value_t, value_t>(1, 0);

int main(int, char **)
{
    // auto w = polynomials::jacobi::getWeightedBasis(1, 1, 3);
    // auto wp = polynomials::jacobi::getWeightedBasisDerivatives(1, 1, 3);
    // auto wpp = polynomials::jacobi::getWeightedBasisSecondDerivatives(1, 1, 3);
    // value_t x = 0.3;
    // int k = 2;
    // std::cout << w[k](x) << " " << wp[k](x) << " " << wpp[k](x) << std::endl;

    // auto y = [&](value_t x) {
    //     return (p(x) * wpp[0](x) + q(x) * wp[0](x) + r(x) * w[0](x)) * w[0](x);
    // };
    // std::cout << functions::calculateIntegralUsing::compound::middleRect(y, -1, 1, 20) << std::endl;

    // return 0;

    int n = 3;
    auto solution = diff::solveUsing::galerkin(
        p,
        q,
        r,
        f,
        f_prime,
        f_prime_prime,
        polynomials::jacobi::getWeightedBasis(1, 1, n),
        polynomials::jacobi::getWeightedBasisDerivatives(1, 1, n),
        polynomials::jacobi::getWeightedBasisSecondDerivatives(1, 1, n),
        leftBoundConditions,
        rightBoundConditions
    );

    int h = 10;
    io::printTable(2, h + 1, {"x", "u(x)"}, [h, &solution](int row, int col) {
        value_t x = -1.0 + 2.0 / (value_t)h * row;
        switch (col) {
        case 0:
            return x;
        case 1:
            return solution(x);
        default:
            return nanl("impossible");
        }
    });
}
