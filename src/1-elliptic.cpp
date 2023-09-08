#include "../lib/io.hpp"
#include "../lib/math.hpp"
#include "../lib/matrix.hpp"
#include <cmath>
#include <iostream>
#include <numbers>
#include <utility>

using math::two_arg_function_t;
using math::value_t;

constexpr value_t epsilon = 1e-3;
constexpr value_t lx = 1, ly = 1;
constexpr value_t c1 = 2, c2 = 5;
constexpr value_t d1 = 1, d2 = 1;
constexpr int N = 5, M = 5;
constexpr value_t hx = lx / value_t(N);
constexpr value_t hy = ly / value_t(M);

value_t f(value_t x, value_t y)
{
    return -(4.0 * x * x * y + 2.0 * x * x * (y + 1.0) + 6.0 * x * y * y * (y + 1) + 2.0 * (3.0 * x + 2.0) * y * y * (y + 1.0));
}

value_t p(value_t x, value_t y)
{
    return 3.0 * x + 2.0;
}

value_t q(value_t x, value_t y)
{
    return 1.0;
}

value_t mu(value_t x, value_t y)
{
    if (x == 0 || x == lx || y == 0 || y == ly) {
        return x * x * y * y * (1.0 + y);
    }
    return 0;
}

value_t u_exact(value_t x, value_t y)
{
    return x * x * y * y * (1.0 + y);
}

using std::numbers::pi;
const value_t delta = c1 * 4.0 * std::pow(std::sin(pi * hx / (2 * lx)), 2) / (hx * hx) +
                      d1 * 4.0 * std::pow(std::sin(pi * hy / (2 * ly)), 2) / (hy * hy);
const value_t Delta = c2 * 4.0 * std::pow(std::cos(pi * hx / (2 * lx)), 2) / (hx * hx) +
                      d2 * 4.0 * std::pow(std::cos(pi * hy / (2 * ly)), 2) / (hy * hy);
const value_t xi = delta / Delta;
const value_t rho = (Delta - delta) / (Delta + delta);

value_t norm(math::Matrix m);
value_t Lu(const math::Matrix &U, value_t i, value_t j);
math::Matrix Lu(const math::Matrix &U);
math::Matrix Lu(two_arg_function_t u);
math::Matrix discretize(two_arg_function_t u);
void simpleIteration(const math::Matrix &_U0, const math::Matrix &Uexact, const math::Matrix &F);

int main()
{

    math::Matrix F(M + 1, N + 1);
    for (unsigned int i = 0; i < M + 1; i++) {
        for (unsigned int j = 0; j < N + 1; j++) {
            F(i, j) = f(i * hx, j * hy);
        }
    }

    math::Matrix U0 = discretize(mu);
    math::Matrix LU0 = Lu(U0);
    math::Matrix Uexact = discretize(u_exact);
    math::Matrix LUexact = Lu(Uexact);

    std::cout << "Approximation measure: ||F-Lu*||=" << norm(F + LUexact) << std::endl;
    std::cout << "Discrepancy norm:      ||F-AU0||=" << norm(F + LU0) << std::endl;
    std::cout << "Specral radius:           rho(H)=" << rho << std::endl;
    std::cout << "Specral radius squared: rho^2(H)=" << rho * rho << std::endl;

    std::cout << std::endl;
    simpleIteration(U0, Uexact, F);
}

value_t norm(math::Matrix m)
{
    value_t result = 0;
    for (unsigned int i = 1; i < m.m_width - 1; i++) {
        for (unsigned int j = 1; j < m.m_height - 1; j++) {
            value_t v = std::abs(m(i, j));
            if (v > result) result = v;
        }
    }
    return result;
}

value_t v(two_arg_function_t fn, value_t i, value_t j)
{
    return fn(hx * i, hy * j);
}

math::Matrix discretize(two_arg_function_t u)
{
    math::Matrix U(M + 1, N + 1);
    for (unsigned int i = 0; i < M + 1; i++) {
        for (unsigned int j = 0; j < N + 1; j++) {
            U(i, j) = u(hx * i, hy * j);
        }
    }
    return U;
}
value_t Lu(const math::Matrix &U, value_t i, value_t j)
{
    return (
        v(p, i + 0.5, j) * (U(i + 1, j) - U(i, j)) / (hx * hx) - v(p, i - 0.5, j) * (U(i, j) - U(i - 1, j)) / (hx * hx) +
        v(q, i, j + 0.5) * (U(i, j + 1) - U(i, j)) / (hy * hy) - v(q, i, j - 0.5) * (U(i, j) - U(i, j - 1)) / (hy * hy)
    );
}
math::Matrix Lu(const math::Matrix &U)
{
    math::Matrix AU = U;
    for (unsigned int i = 1; i < M; i++) {
        for (unsigned int j = 1; j < N; j++) {
            AU(i, j) = Lu(U, i, j);
        }
    }
    return AU;
}
math::Matrix Lu(two_arg_function_t u)
{
    math::Matrix U = discretize(u);
    return Lu(U);
}

void simpleIteration(const math::Matrix &U0, const math::Matrix &Uexact, const math::Matrix &F)
{
    const int it_estimate = std::log(1 / epsilon) / (2 * xi);
    const int it_count = 10;

    std::cout << ">>> Simple iteration" << std::endl;
    std::cout << "Iteration count estimate for epsilon=" << epsilon << ":  " << it_estimate << std::endl;

    auto v = [](const two_arg_function_t fn, value_t i, value_t j) {
        return fn(hx * i, hy * j);
    };

    math::Matrix _U0(U0);
    math::Matrix _U1(U0);
    math::Matrix *Uk = &_U0;
    math::Matrix *Uk1 = &_U1;

    value_t F_AU0 = norm(F + Lu(*Uk));
    value_t U0_Ue = norm(*Uk - Uexact);
    value_t prev_Uk_Uk1 = -1;
    value_t prev_rhok = std::nanl("-");

    io::startTable(8, {"k", "||F-AUk||", "rel.d.", "||Uk-u*||", "rel.err.", "||Uk-Uk-1||", "apost.est.", "rho_k"});

    for (int k = 1; k <= it_count; k++) {
        for (unsigned int i = 1; i < Uk->m_height - 1; i++) {
            for (unsigned int j = 1; j < Uk->m_width - 1; j++) {
                value_t k1 = v(p, i - 0.5, j) / (hx * hx);
                value_t k2 = v(p, i + 0.5, j) / (hx * hx);
                value_t k3 = v(q, i, j - 0.5) / (hy * hy);
                value_t k4 = v(q, i, j + 0.5) / (hy * hy);
                (*Uk1)(i, j) =
                    (k1 * (*Uk)(i - 1, j) + k2 * (*Uk)(i + 1, j) + k3 * (*Uk)(i, j - 1) + k4 * (*Uk)(i, j + 1) + v(f, i, j)) /
                    (k1 + k2 + k3 + k4);
            }
        }

        value_t F_AUk = norm(F + Lu(*Uk1));
        value_t Uk_Ue = norm(*Uk1 - Uexact);
        value_t Uk_Uk1 = norm(*Uk1 - *Uk);
        value_t rhok = prev_Uk_Uk1 < 0 ? std::nanl("-") : Uk_Uk1 / prev_Uk_Uk1;
        value_t rho_est = std::sqrt(rhok * prev_rhok);
        io::printRow(
            8,
            {static_cast<value_t>(k), F_AUk, F_AUk / F_AU0, Uk_Ue, Uk_Ue / U0_Ue, Uk_Uk1, rho * Uk_Uk1 / (1 - rho), rho_est}
        );

        prev_Uk_Uk1 = Uk_Uk1;
        prev_rhok = rhok;
        std::swap(Uk, Uk1);
    }
    io::endTable(8);

    std::vector<std::string> headers {"y\\x"};
    for (int i = 0; i <= N; i++) {
        char buffer[20];
        std::snprintf(buffer, 20, "%.1Lf", i * hx);
        headers.push_back("U0[" + std::string(buffer) + "]");
    }
    headers.push_back("");
    for (int i = 0; i <= N; i++) {
        char buffer[20];
        std::snprintf(buffer, 20, "%.1Lf", i * hx);
        headers.push_back("Uk[" + std::string(buffer) + "]");
    }

    io::printTable(1 + Uexact.m_width + 1 + Uk->m_width, Uexact.m_height, 12, headers, [&Uexact, Uk](size_t row, size_t col) {
        if (col == 0) {
            return row * hy;
        }
        if (col <= Uexact.m_width) {
            return Uexact(row, col - 1);
        }
        if (col == Uexact.m_width + 1) {
            return std::nanl("-");
        }
        return (*Uk)(row, col - Uexact.m_width - 2);
    });
}
