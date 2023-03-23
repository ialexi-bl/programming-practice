#pragma once
#include <functional>

namespace math
{
    using value_t = long double;
    using simple_function_t = std::function<value_t(value_t)>;

    simple_function_t operator+(simple_function_t f, simple_function_t g);
    simple_function_t operator-(simple_function_t f, simple_function_t g);
    simple_function_t operator*(simple_function_t f, simple_function_t g);
    simple_function_t operator/(simple_function_t f, simple_function_t g);

    template <typename T> inline int sign(T a)
    {
        return a < 0 ? -1 : 1;
    }
} // namespace math