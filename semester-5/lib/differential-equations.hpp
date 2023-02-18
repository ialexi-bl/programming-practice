#pragma once
#include <functional>
#include <utility>
#include <vector>

namespace diff
{
    using value_t = long double;
    using function = std::function<value_t(value_t)>;
    using coefs_list = std::tuple<value_t, value_t, value_t, value_t>;

    std::vector<coefs_list> initialCoefsSimple(
        int n,
        value_t h,
        std::tuple<value_t, value_t> bounds,
        std::tuple<value_t, value_t, value_t> rightBoundConditions,
        std::tuple<value_t, value_t, value_t> leftBoundConditions,
        std::tuple<function, function, function, function> fns
    );

    std::vector<coefs_list> initialCoefsPrecise(
        int n,
        value_t h,
        std::tuple<value_t, value_t> bounds,
        std::tuple<value_t, value_t, value_t> rightBoundConditions,
        std::tuple<value_t, value_t, value_t> leftBoundConditions,
        std::tuple<function, function, function, function> fns
    );

    std::tuple<std::vector<value_t>, std::vector<value_t>, std::vector<value_t>>
    solveTridiagonalMatrix(std::vector<coefs_list> coefs);

} // namespace diff