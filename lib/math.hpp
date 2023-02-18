#pragma once
#include <functional>

namespace math
{
    using value_t = long double;
    using simple_function_t = std::function<value_t(value_t)>;
}