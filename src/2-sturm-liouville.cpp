#include "../lib/functions.hpp"
#include "../lib/math.hpp"
#include "../lib/matrix.hpp"
#include "../lib/polynomials.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>

using math::simple_function_t;
using math::value_t;

constexpr int n = 7;
constexpr value_t k = 1.57894;
constexpr value_t l = 8.59453;

value_t p(value_t x)
{
    return k * x + l;
}

value_t q(value_t x)
{
    return k * k * (1.0 / (k * x + l) - k * x);
}

static value_t get_norm_coefficient(int a, int b, int n)
{
    // Maybe only be correct for a=b=2
    return (1.0 / 4.0) * std::sqrt((n + 3.0) * (n + 4.0) * (2.0 * n + 5.0) / (2.0 * (n + 1.0) * (n + 2.0)));
}
static simple_function_t getBasisElement(int a, int b, int n)
{
    return [=](value_t x) {
        return get_norm_coefficient(a, b, n) * (1.0 - x * x) * math::polynomials::jacobi::evaluate(a, b, n, x);
    };
}
static simple_function_t getBasisElementDerivative(int a, int b, int n)
{
    return [=](value_t x) {
        return get_norm_coefficient(a, b, n) * (-2.0 * x * math::polynomials::jacobi::evaluate(a, b, n, x) +
                                                (1.0 - x * x) * math::polynomials::jacobi::evaluateDerivative(a, b, n, x));
    };
}

std::vector<simple_function_t> getBasis()
{
    std::vector<simple_function_t> basis;
    for (int i = 0; i < n; i++) {
        basis.push_back(getBasisElement(2, 2, i));
    }
    return basis;
}
std::vector<simple_function_t> getBasisDerivatives()
{
    std::vector<simple_function_t> derivatives;
    for (int i = 0; i < n; i++) {
        derivatives.push_back(getBasisElementDerivative(2, 2, i));
    }
    return derivatives;
}

int main()
{
    auto basis = getBasis();
    auto basis_derivatives = getBasisDerivatives();

    auto energy_scalar_product = [](simple_function_t y, simple_function_t y_prime, simple_function_t z, simple_function_t z_prime
                                 ) {
        using namespace math;
        return math::calculateIntegralUsing::compound::middleRect(p * y_prime * z_prime + q * y * z, -1, 1, 100);
    };

    math::Matrix GL(n, n);
    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            GL(i, j) = GL(j, i) = energy_scalar_product(basis[i], basis_derivatives[i], basis[j], basis_derivatives[j]);
        }
    }

    auto eigenValues = math::getJacobiEigenvectors(GL, 1e-6);

    std::cout << GL << std::endl;
    // std::cout << math::for_wolfram(GL) << std::endl;
    std::cout << "Eigenvalues:" << std::endl;

    for (auto &[value, vec] : eigenValues) {
        std::cout << "- " << value << std::endl;
    }
}
