#include "math.hpp"

namespace math
{
    simple_function_t operator+(simple_function_t f, simple_function_t g)
    {
        return [=](value_t x) {
            return f(x) + g(x);
        };
    }
    simple_function_t operator-(simple_function_t f, simple_function_t g)
    {
        return [=](value_t x) {
            return f(x) - g(x);
        };
    }
    simple_function_t operator*(simple_function_t f, simple_function_t g)
    {
        return [=](value_t x) {
            return f(x) * g(x);
        };
    }
    simple_function_t operator/(simple_function_t f, simple_function_t g)
    {
        return [=](value_t x) {
            return f(x) / g(x);
        };
    }
} // namespace math