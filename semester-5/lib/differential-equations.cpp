#include "differential-equations.hpp"

namespace diff
{
    std::vector<coefs_list> initialCoefsSimple(
        int n,
        value_t h,
        std::tuple<value_t, value_t> bounds,
        std::tuple<value_t, value_t, value_t> rightBoundConditions,
        std::tuple<value_t, value_t, value_t> leftBoundConditions,
        std::tuple<function, function, function, function> fns
    )
    {
        auto [a, b] = bounds;
        auto [a1, a2, a0] = rightBoundConditions;
        auto [b1, b2, b0] = leftBoundConditions;
        auto [p, q, r, f] = fns;

        std::vector<coefs_list> coefs(n);

        coefs[0] = {0, h * a1 + a2, a2, -h * a0};
        coefs[n - 1] = {b2, h * b1 + b2, 0, -h * b0};

        for (int i = 1; i < n - 1; i++) {
            value_t xi = a + i * h;
            value_t xip2 = xi - h / 2.0;
            value_t xim2 = xi + h / 2.0;

            value_t Ai = -p(xim2) - q(xi) * h / 2.0;
            value_t Bi = -p(xip2) + q(xi) * h / 2.0;
            value_t Ci = Ai + Bi - h * h * r(xi);
            value_t Gi = h * h * f(xi);

            coefs[i] = {Ai, Bi, Ci, Gi};
        }

        return coefs;
    }

    std::vector<coefs_list> initialCoefsPrecise(
        int n,
        value_t h,
        std::tuple<value_t, value_t> bounds,
        std::tuple<value_t, value_t, value_t> rightBoundConditions,
        std::tuple<value_t, value_t, value_t> leftBoundConditions,
        std::tuple<function, function, function, function> fns
    )
    {
        auto [a, b] = bounds;
        auto [a0, a1, a2] = rightBoundConditions;
        auto [b0, b1, b2] = leftBoundConditions;
        auto [p, q, r, f] = fns;

        std::vector<coefs_list> coefs(n + 1);

        coefs[0] = {0, h * a1 + 2 * a2, 2 * a2 - h * a1, -2 * h * a0};
        coefs[n] = {2 * b2 - h * b1, h * b1 + 2 * b2, 0, -2 * h * b0};

        for (int i = 1; i < n; i++) {
            value_t xi = a + i * h;
            value_t xip2 = xi - h / 2.0;
            value_t xim2 = xi + h / 2.0;

            value_t Ai = -p(xim2) - q(xi) * h / 2.0;
            value_t Bi = -p(xip2) + q(xi) * h / 2.0;
            value_t Ci = Ai + Bi - h * h * r(xi);
            value_t Gi = h * h * f(xi);

            coefs[i] = {Ai, Bi, Ci, Gi};
        }

        return coefs;
    }

    std::tuple<std::vector<value_t>, std::vector<value_t>, std::vector<value_t>>
    solveTridiagonalMatrix(std::vector<coefs_list> coefs)
    {
        size_t n = coefs.size();
        if (n == 0) {
            return {};
        }

        std::vector<value_t> S(n), T(n);
        {
            auto [A0, B0, C0, G0] = coefs[0];
            S[0] = C0 / B0;
            T[0] = -G0 / B0;

            for (size_t i = 1; i < n; i++) {
                auto [Ai, Bi, Ci, Gi] = coefs[i];
                auto denominator = (Bi - Ai * S[i - 1]);
                S[i] = Ci / denominator;
                T[i] = (Ai * T[i - 1] - Gi) / denominator;
            }
        }

        std::vector<value_t> Y(n);
        {
            Y[n - 1] = T[n - 1];

            for (int i = n - 2; i >= 0; i--) {
                Y[i] = S[i] * Y[i + 1] + T[i];
            }
        }

        return {Y, S, T};
    }
} // namespace diff