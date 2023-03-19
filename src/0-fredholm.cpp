#include "../lib/functions.hpp"
#include "../lib/io.hpp"
#include "../lib/math.hpp"
#include "../lib/matrix.hpp"
#include <cmath>
#include <iostream>

using math::simple_function_t;
using math::value_t;

value_t H(value_t x, value_t y)
{
    return -0.5 * std::exp((x - 0.5) * y * y);
}

value_t f(value_t x)
{
    return x + 0.5;
}

constexpr value_t a = 0;
constexpr value_t b = 1;
constexpr value_t epsilon = 1e-3;

static simple_function_t solve(int n)
{
    std::vector<std::pair<value_t, value_t>> nodes;
    for (int i = 0; i < n; i++) nodes.push_back({a + (b - a) / n * i, (b - a) / n});

    math::Matrix D(n, n);
    math::vector g(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            D(i, j) = i == j ? 1 : 0;
            D(i, j) -= nodes[j].second * H(nodes[i].first, nodes[j].first);
        }
        g[i] = f(nodes[i].first);
    }

    math::vector z = math::solveLinearSystem(D, g);

    return [=](value_t x) -> value_t {
        value_t result = f(x);
        for (int i = 0; i < n; i++) {
            result += nodes[i].second * H(x, nodes[i].first) * z[i];
        }
        return result;
    };
}

int main()
{
    constexpr value_t mid = (a + b) / 2.0;
    std::vector<std::tuple<value_t, value_t, value_t>> table;
    std::tuple<value_t, value_t, value_t> error;

    for (int n = 2; n <= 1.5e3; n *= 2) {
        std::cout << n << std::endl;
        auto solution = solve(n);
        table.push_back(std::make_tuple(solution(a), solution(mid), solution(b)));

        size_t size = table.size();
        if (size > 1) {
            auto [p1, p2, p3] = table[size - 2];
            auto [c1, c2, c3] = table[size - 1];

            value_t e1 = std::abs(p1 - c1);
            value_t e2 = std::abs(p2 - c2);
            value_t e3 = std::abs(p3 - c3);

            error = std::make_tuple(e1, e2, e3);
            if (e1 < epsilon && e2 < epsilon && e3 < epsilon) {
                break;
            }
        }
    }

    size_t size = table.size();
    io::printTable(4, size + 1, {"x", "u(a)", "u(m)", "u(b)"}, [&](int row, int col) -> value_t {
        if (row < size) {
            auto [u1, u2, u3] = table[row];
            switch (col) {
            case 0:
                return 2 + row;
            case 1:
                return u1;
            case 2:
                return u2;
            case 3:
                return u3;
            }
            return std::nanl("impossible");
        }
        switch (col) {
        case 0:
            return 0;
        case 1:
            return std::get<0>(error);
        case 2:
            return std::get<1>(error);
        case 3:
            return std::get<2>(error);
        }
        return std::nanl("impossible");
    });
}