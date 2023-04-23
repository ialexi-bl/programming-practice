#include "../lib/functions.hpp"
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
}

solution_t getExactSolution(simple_function_t f)
{
    using std::numbers::pi;
    using std::numbers::sqrt2;

    constexpr value_t epsilon = 1e-10;
    std::vector<value_t> c;

    return [c, f](value_t x, value_t t) mutable {
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

            if (term < epsilon) {
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

    value_t h = 1 / N;

    std::vector<value_t> c;
    for (size_t i = 0; i < N - 1; i++) {
        value_t ci = 0;

        for (size_t j = 1; j <= N - 1; j++) {
            ci += f(j * h) * std::sin((i + 1) * pi * j * h);
        }

        c.push_back(sqrt2 * h * ci);
    }

    return [c, f, N](value_t x, value_t t) {
        value_t sum = 0;

        for (size_t i = 0; i < N - 1; i++) {
            sum += c[i] * std::exp(-pi * pi * (i + 1) * (i + 1) * t) * std::sin((i + 1) * pi * x);
        }

        return sqrt2 * sum;
    };
}

int main()
{
    solution_t exact_solution = getExactSolution(f);
}