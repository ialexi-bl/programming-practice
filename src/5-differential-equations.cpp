#include "lib/differential-equations.hpp"
#include "lib/io.hpp"
#include <cmath>
#include <iostream>

using diff::value_t;

value_t p(value_t x)
{
    return (x + 3.0) / (2.0 + x);
}

value_t q(value_t x)
{
    return x;
}

value_t r(value_t x)
{
    return std::exp(x / 3.0);
}

value_t f(value_t x)
{
    return 1.0 + x / 2.0;
}

static auto bounds = std::make_tuple(-1.0, 1.0);
static auto rightBoundConditions = std::make_tuple(0, -1, 0);
static auto leftBoundConditions = std::make_tuple(0, 1, 0);

static int n = 10;
static value_t h = (std::get<1>(bounds) - std::get<0>(bounds)) / n;

int main()
{
    auto fns = std::make_tuple(p, q, r, f);

    {
        auto coefs = diff::initialCoefsSimple(n, h, bounds, rightBoundConditions, leftBoundConditions, fns);
        auto [Y, S, T] = diff::solveTridiagonalMatrix(coefs);

        std::cout << "O(h):" << std::endl;
        io::printTable(8, n, {"xi", "Ai", "Bi", "Ci", "Gi", "si", "ti", "yi"}, [&](int row, int col) {
            switch (col) {
            case 0:
                return std::get<0>(bounds) + h * row;
            case 1:
                return std::get<0>(coefs[row]);
            case 2:
                return std::get<1>(coefs[row]);
            case 3:
                return std::get<2>(coefs[row]);
            case 4:
                return std::get<3>(coefs[row]);
            case 5:
                return S[row];
            case 6:
                return T[row];
            case 7:
                return Y[row];
            default:
                return std::nanl("impossible");
            }
        });
    }

    {
        auto coefs = diff::initialCoefsPrecise(n, h, bounds, rightBoundConditions, leftBoundConditions, fns);
        auto [Y, S, T] = diff::solveTridiagonalMatrix(coefs);

        std::cout << "O(h²):" << std::endl;
        io::printTable(8, n, {"xi", "Ai", "Bi", "Ci", "Gi", "si", "ti", "yi"}, [&](int row, int col) {
            switch (col) {
            case 0:
                return std::get<0>(bounds) + h * row;
            case 1:
                return std::get<0>(coefs[row]);
            case 2:
                return std::get<1>(coefs[row]);
            case 3:
                return std::get<2>(coefs[row]);
            case 4:
                return std::get<3>(coefs[row]);
            case 5:
                return S[row];
            case 6:
                return T[row];
            case 7:
                return Y[row];
            default:
                return std::nanl("impossible");
            }
        });
    }
}