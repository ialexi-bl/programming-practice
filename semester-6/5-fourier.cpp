#include "../lib/functions.hpp"
#include "../lib/io.hpp"
#include "../lib/math.hpp"
#include <cmath>
#include <functional>
#include <iostream>
#include <numbers>

using math::simple_function_t;
using math::value_t;

using solution_t = std::function<value_t(value_t, value_t)>;

value_t f(value_t x)
{
    return std::sin(3 * std::numbers::pi * x);
    // return x * (1.0 - x);
}

solution_t getExactSolution(simple_function_t f)
{
    using std::numbers::pi;
    using std::numbers::sqrt2;

    constexpr value_t epsilon = 1e-40;
    std::vector<value_t> c;

    return [=](value_t x, value_t t) mutable {
        value_t sum = 0;

        for (size_t i = 0; i < 100; i++) {
            if (c.size() <= i) {
                value_t ci = math::calculateIntegralUsing::compound::simpson(
                    [f, i](value_t x) {
                        return sqrt2 * f(x) * std::sin((i + 1) * pi * x);
                    },
                    0,
                    1,
                    100
                );
                c.push_back(ci);
            }

            value_t term = c[i] * std::exp(-pi * pi * (i + 1) * (i + 1) * t) * std::sin((i + 1) * pi * x);
            sum += term;

            if (std::abs(term) < epsilon) {
                break;
            }
        }

        return sqrt2 * sum;
    };
}

solution_t getDiscreteFourierSolution(simple_function_t f, unsigned int N)
{
    using std::numbers::pi;
    using std::numbers::sqrt2;

    value_t h = 1.0 / N;

    std::vector<value_t> c;
    for (size_t p = 1; p <= N - 1; p++) {
        value_t cp = 0;

        for (size_t i = 1; i <= N - 1; i++) {
            cp += f(i * h) * std::sin(p * pi * i * h);
        }

        c.push_back(sqrt2 * h * cp);
    }

    return [=](value_t x, value_t t) {
        value_t sum = 0;

        for (size_t p = 1; p <= N - 1; p++) {
            sum += c[p - 1] * std::exp(-pi * pi * p * p * t) * std::sin(p * pi * x);
        }

        return sqrt2 * sum;
    };
}

solution_t getGridSolution(simple_function_t f, value_t sigma, unsigned int N, value_t tau)
{
    using std::numbers::pi;
    using std::numbers::sqrt2;

    value_t h = 1.0 / N;

    std::vector<value_t> l;
    for (size_t p = 1; p <= N - 1; p++) {
        value_t sin = std::sin(p * pi * h / 2.0);
        value_t common_coef = 4.0 * tau * sin * sin / (h * h);
        l.push_back((1.0 - (1.0 - sigma) * common_coef) / (1.0 + sigma * common_coef));
    }

    std::vector<value_t> c;
    for (size_t p = 1; p <= N - 1; p++) {
        value_t cp = 0;

        for (size_t i = 1; i <= N - 1; i++) {
            cp += f(i * h) * std::sin(p * pi * i * h);
        }

        c.push_back(sqrt2 * h * cp);
    }

    return [=](value_t x, value_t t) {
        unsigned int k = std::round(t / tau);
        value_t sum = 0;

        for (size_t p = 1; p <= N - 1; p++) {
            sum += c[p - 1] * std::pow(l[p - 1], k) * std::sin(pi * p * x);
        }

        return sqrt2 * sum;
    };
}

int main()
{
    constexpr unsigned int N = 10;
    constexpr value_t T = 0.1;
    constexpr value_t tau = T / 20.0;

    solution_t exact_solution = getExactSolution(f);
    solution_t discrete_fourier_solution = getDiscreteFourierSolution(f, N);
    solution_t grid_explicit_solution = getGridSolution(f, 0.0, N, tau);
    solution_t grid_implicit_solution = getGridSolution(f, 1.0, N, tau);
    solution_t grid_s12_solution = getGridSolution(f, 0.5, N, tau);

    auto printSolution = [](solution_t solution) {
        io::printTable(7, 6, 15, {"t\\x", "0", "0.2", "0.4", "0.6", "0.8", "1"}, [=](size_t row, size_t col) {
            switch (col) {
            case 0:
                return row * tau;
            default:
                return solution((col - 1) * 0.2, row * tau);
            }
        });
    };

    std::cout << "> Exact solution" << std::endl;
    printSolution(exact_solution);
    std::cout << "> Discrete Fourier solution" << std::endl;
    printSolution(discrete_fourier_solution);
    std::cout << "> Grid solution σ=0 (explicit)" << std::endl;
    printSolution(grid_explicit_solution);
    std::cout << "> Grid solution σ=1 (implicit)" << std::endl;
    printSolution(grid_implicit_solution);
    std::cout << "> Grid solution σ=1/2" << std::endl;
    printSolution(grid_s12_solution);
}
