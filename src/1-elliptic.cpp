#include "../lib/io.hpp"
#include "../lib/math.hpp"
#include "../lib/matrix.hpp"
#include <cmath>
#include <functional>
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
void simpleIterationOptimal(const math::Matrix &_U0, const math::Matrix &Uexact, const math::Matrix &F);
void seidel(const math::Matrix &_U0, const math::Matrix &Uexact, const math::Matrix &F);
void upperRelaxation(const math::Matrix &_U0, const math::Matrix &Uexact, const math::Matrix &F);
void chebyshevParams(const math::Matrix &_U0, const math::Matrix &Uexact, const math::Matrix &F);
void alternatingTriangles(const math::Matrix &_U0, const math::Matrix &Uexact, const math::Matrix &F);
void alternatingTrianglesChebyshev(const math::Matrix &_U0, const math::Matrix &Uexact, const math::Matrix &F);
void alternatingDirections(const math::Matrix &_U0, const math::Matrix &Uexact, const math::Matrix &F);

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

    // std::cout << std::endl;
    // simpleIteration(U0, Uexact, F);
    // std::cout << std::endl;
    // simpleIterationOptimal(U0, Uexact, F);
    // std::cout << std::endl;
    // seidel(U0, Uexact, F);
    // std::cout << std::endl;
    // upperRelaxation(U0, Uexact, F);
    // std::cout << std::endl;
    // chebyshevParams(U0, Uexact, F);
    // std::cout << std::endl;
    // alternatingTriangles(U0, Uexact, F);
    // std::cout << std::endl;
    // alternatingTrianglesChebyshev(U0, Uexact, F);
    std::cout << std::endl;
    alternatingDirections(U0, Uexact, F);
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

void printTwoSolutions(const math::Matrix &Uexact, const math::Matrix &U)
{
    std::vector<std::string> headers {"y\\x"};
    for (int i = 0; i <= N; i++) {
        char buffer[20];
        std::snprintf(buffer, 20, "%.1Lf", i * hx);
        headers.push_back("U*[" + std::string(buffer) + "]");
    }
    headers.push_back("");
    for (int i = 0; i <= N; i++) {
        char buffer[20];
        std::snprintf(buffer, 20, "%.1Lf", i * hx);
        headers.push_back("Uk[" + std::string(buffer) + "]");
    }

    io::printTable(1 + Uexact.m_width + 1 + U.m_width, Uexact.m_height, 12, headers, [&Uexact, &U](size_t row, size_t col) {
        if (col == 0) {
            return row * hy;
        }
        if (col <= Uexact.m_width) {
            return Uexact(row, col - 1);
        }
        if (col == Uexact.m_width + 1) {
            return std::nanl("-");
        }
        return U(row, col - Uexact.m_width - 2);
    });
}

void runIterations(
    const math::Matrix &U0,
    const math::Matrix &Uexact,
    const math::Matrix &F,
    int it_count,
    std::function<value_t(int k, const math::Matrix &Uprev, const math::Matrix &Ucurr, size_t i, size_t j)> recalculate
)
{
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
        for (size_t i = 1; i < Uk->m_height - 1; i++) {
            for (size_t j = 1; j < Uk->m_width - 1; j++) {
                (*Uk1)(i, j) = recalculate(k, *Uk, *Uk1, i, j);
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

    printTwoSolutions(Uexact, *Uk);
}

void simpleIteration(const math::Matrix &U0, const math::Matrix &Uexact, const math::Matrix &F)
{
    const int it_estimate = std::log(1 / epsilon) / (2 * xi);
    const int it_count = 10;

    std::cout << ">>> Simple iteration" << std::endl;
    std::cout << "Iteration count estimate for epsilon=" << epsilon << ":  " << it_estimate << std::endl;

    runIterations(U0, Uexact, F, it_count, [&F](int k, const math::Matrix &U, const math::Matrix &Ucurr, size_t i, size_t j) {
        value_t k1 = v(p, i - 0.5, j) / (hx * hx);
        value_t k2 = v(p, i + 0.5, j) / (hx * hx);
        value_t k3 = v(q, i, j - 0.5) / (hy * hy);
        value_t k4 = v(q, i, j + 0.5) / (hy * hy);
        return (k1 * U(i - 1, j) + k2 * U(i + 1, j) + k3 * U(i, j - 1) + k4 * U(i, j + 1) + F(i, j)) / (k1 + k2 + k3 + k4);
    });
}

void simpleIterationOptimal(const math::Matrix &U0, const math::Matrix &Uexact, const math::Matrix &F)
{
    const int it_estimate = std::log(1.0l / epsilon) / (2.0l * xi);
    const int it_count = 10;

    std::cout << ">>> Simple iteration with optimal parameter" << std::endl;
    std::cout << "Iteration count estimate for epsilon=" << epsilon << ":  " << it_estimate << std::endl;

    const value_t tau = 2 / (delta + Delta);
    runIterations(
        U0,
        Uexact,
        F,
        it_count,
        [tau, &F](int k, const math::Matrix &U, const math::Matrix &Ucurr, size_t i, size_t j) {
            return U(i, j) + tau * (v(p, i + 0.5, j) * (U(i + 1, j) - U(i, j)) / (hx * hx) -
                                    v(p, i - 0.5, j) * (U(i, j) - U(i - 1, j)) / (hx * hx) +
                                    v(q, i, j + 0.5) * (U(i, j + 1) - U(i, j)) / (hy * hy) -
                                    v(q, i, j - 0.5) * (U(i, j) - U(i, j - 1)) / (hy * hy) + F(i, j));
        }
    );
}

void seidel(const math::Matrix &U0, const math::Matrix &Uexact, const math::Matrix &F)
{
    const int it_estimate = std::log(1.0l / epsilon) / (4.0l * xi);
    const int it_count = 10;

    std::cout << ">>> Seidel" << std::endl;
    std::cout << "Iteration count estimate for epsilon=" << epsilon << ":  " << it_estimate << std::endl;

    const value_t tau = 2 / (delta + Delta);
    runIterations(
        U0,
        Uexact,
        F,
        it_count,
        [tau, &F](int k, const math::Matrix &U, const math::Matrix &Ucurr, size_t i, size_t j) {
            value_t k1 = v(p, i - 0.5, j) / (hx * hx);
            value_t k2 = v(p, i + 0.5, j) / (hx * hx);
            value_t k3 = v(q, i, j - 0.5) / (hy * hy);
            value_t k4 = v(q, i, j + 0.5) / (hy * hy);
            return (k1 * Ucurr(i - 1, j) + k2 * U(i + 1, j) + k3 * Ucurr(i, j - 1) + k4 * U(i, j + 1) + F(i, j)) /
                   (k1 + k2 + k3 + k4);
        }
    );
}

void upperRelaxation(const math::Matrix &U0, const math::Matrix &Uexact, const math::Matrix &F)
{
    const int it_estimate = std::log(1.0l / epsilon) / std::sqrt(xi);
    const int it_count = 10;

    std::cout << ">>> Upper relaxation" << std::endl;
    std::cout << "Iteration count estimate for epsilon=" << epsilon << ":  " << it_estimate << std::endl;

    const value_t omega = 2.0l / (1.0l + std::sqrt(1 - rho * rho));
    runIterations(
        U0,
        Uexact,
        F,
        it_count,
        [omega, &F](int k, const math::Matrix &U, const math::Matrix &Ucurr, size_t i, size_t j) {
            value_t k1 = v(p, i - 0.5, j) / (hx * hx);
            value_t k2 = v(p, i + 0.5, j) / (hx * hx);
            value_t k3 = v(q, i, j - 0.5) / (hy * hy);
            value_t k4 = v(q, i, j + 0.5) / (hy * hy);

            return U(i, j) + omega *
                                 (F(i, j) + k2 * (U(i + 1, j) - U(i, j)) - k1 * (U(i, j) - Ucurr(i - 1, j)) +
                                  k4 * (U(i, j + 1) - U(i, j)) - k3 * (U(i, j) - Ucurr(i, j - 1))) /
                                 (k1 + k2 + k3 + k4);
        }
    );
}

void chebyshevParams(const math::Matrix &U0, const math::Matrix &Uexact, const math::Matrix &F)
{
    const int it_estimate = std::log(2.0l / epsilon) / (2 * std::sqrt(xi));
    const int it_count = 16;

    std::cout << ">>> Chebyshev params" << std::endl;
    std::cout << "Iteration count estimate for epsilon=" << epsilon << ":  " << it_estimate << std::endl;

    std::vector<int> theta(it_count, 1);
    for (size_t m = 1; m < it_count; m *= 2) {
        for (size_t i = m; i > 0; i--) {
            theta[(2 * i - 1) - 1] = theta[(i)-1];
        }
        for (size_t i = 1; i <= m; i++) {
            theta[(2 * i) - 1] = 4 * m - theta[(2 * i - 1) - 1];
        }
    }

    runIterations(
        U0,
        Uexact,
        F,
        it_count,
        [it_count, &F, &theta](int k, const math::Matrix &U, const math::Matrix &Ucurr, size_t i, size_t j) {
            using std::numbers::pi;
            const value_t tau = 2.0l / (Delta + delta + (Delta - delta) * std::cos(pi * theta[k - 1] / (2.0l * it_count)));

            value_t k1 = v(p, i - 0.5, j) / (hx * hx);
            value_t k2 = v(p, i + 0.5, j) / (hx * hx);
            value_t k3 = v(q, i, j - 0.5) / (hy * hy);
            value_t k4 = v(q, i, j + 0.5) / (hy * hy);

            return U(i, j) + tau * (F(i, j) + k2 * (U(i + 1, j) - U(i, j)) - k1 * (U(i, j) - U(i - 1, j)) +
                                    k4 * (U(i, j + 1) - U(i, j)) - k3 * (U(i, j) - U(i, j - 1)));
        }
    );
}

void fillBorder(math::Matrix &M, std::function<value_t(size_t i, size_t j)> compute)
{
    for (size_t i = 0; i < M.m_height; i++) {
        M(i, 0) = compute(i, 0);
        M(i, M.m_height - 1) = compute(i, M.m_height - 1);
    }
    for (size_t j = 0; j < M.m_width; j++) {
        M(0, j) = compute(0, j);
        M(M.m_width - 1, j) = compute(M.m_width - 1, j);
    }
}

void alternatingTriangles(const math::Matrix &U0, const math::Matrix &Uexact, const math::Matrix &F)
{
    const int it_estimate = std::log(1.0l / epsilon) / std::log(1.0l / rho);
    const int it_count = 10;

    std::cout << ">>> Alternating triangles" << std::endl;
    std::cout << "Iteration count estimate for epsilon=" << epsilon << ":  " << it_estimate << std::endl;

    const value_t omega = 2.0l / std::sqrt(delta * Delta);
    const value_t gamma1 = delta / (2.0l + 2.0l * std::sqrt(xi));
    const value_t gamma2 = delta / (4.0l * std::sqrt(xi));
    const value_t tau = 2.0l / (gamma1 + gamma2);
    const value_t k1 = omega / (hx * hx);
    const value_t k2 = omega / (hy * hy);

    math::Matrix _U0(U0);
    math::Matrix _U1(U0);
    math::Matrix *Uk = &_U0;
    math::Matrix *Uk1 = &_U1;

    value_t F_AU0 = norm(F + Lu(*Uk));
    value_t U0_Ue = norm(*Uk - Uexact);

    io::startTable(6, {"k", "||F-AUk||", "rel.d.", "||Uk-u*||", "rel.err.", "||Uk-Uk-1||"});
    for (int k = 1; k <= it_count; k++) {
        math::Matrix Phi = Lu(*Uk) + F;
        math::Matrix w(*Uk);
        fillBorder(w, [](size_t i, size_t j) {
            return 0.0l;
        });

        for (size_t i = 1; i < Uk->m_height - 1; i++) {
            for (size_t j = 1; j < Uk->m_width - 1; j++) {
                const value_t c1 = k1 * v(p, i - 0.5, j);
                const value_t c2 = k2 * v(q, i, j - 0.5);

                w(i, j) = (c1 * w(i - 1, j) + c2 * w(i, j - 1) + Phi(i, j)) / (1.0 + c1 + c2);
            }
        }

        for (size_t i = Uk->m_height - 2; i > 0; i--) {
            for (size_t j = Uk->m_width - 2; j > 0; j--) {
                const value_t c1 = k1 * v(p, i + 0.5, j);
                const value_t c2 = k2 * v(q, i, j + 0.5);

                w(i, j) = (c1 * w(i + 1, j) + c2 * w(i, j + 1) + w(i, j)) / (1.0 + c1 + c2);
            }
        }

        *Uk1 = *Uk + tau * w;

        value_t F_AUk = norm(F + Lu(*Uk1));
        value_t Uk_Ue = norm(*Uk1 - Uexact);
        value_t Uk_Uk1 = norm(*Uk1 - *Uk);
        io::printRow(6, {static_cast<value_t>(k), F_AUk, F_AUk / F_AU0, Uk_Ue, Uk_Ue / U0_Ue, Uk_Uk1});

        std::swap(Uk, Uk1);
    }
    io::endTable(6);

    printTwoSolutions(Uexact, *Uk);
}

void alternatingTrianglesChebyshev(const math::Matrix &U0, const math::Matrix &Uexact, const math::Matrix &F)
{
    using std::numbers::sqrt2;
    const int it_estimate = std::log(2.0l / epsilon) / (2.0l * sqrt2 * std::pow(xi, 1.0l / 4.0l));
    const int it_count = 10;

    std::cout << ">>> Alternating triangles chebyshev" << std::endl;
    std::cout << "Iteration count estimate for epsilon=" << epsilon << ":  " << it_estimate << std::endl;

    const value_t omega = 2.0l / std::sqrt(delta * Delta);
    const value_t gamma1 = delta / (2.0l + 2.0l * std::sqrt(xi));
    const value_t gamma2 = delta / (4.0l * std::sqrt(xi));
    const value_t k1 = omega / (hx * hx);
    const value_t k2 = omega / (hy * hy);

    math::Matrix _U0(U0);
    math::Matrix _U1(U0);
    math::Matrix *Uk = &_U0;
    math::Matrix *Uk1 = &_U1;

    value_t F_AU0 = norm(F + Lu(*Uk));
    value_t U0_Ue = norm(*Uk - Uexact);

    io::startTable(6, {"k", "||F-AUk||", "rel.d.", "||Uk-u*||", "rel.err.", "||Uk-Uk-1||"});
    for (int k = 1; k <= it_count; k++) {
        math::Matrix Phi = Lu(*Uk) + F;
        math::Matrix w(*Uk);
        fillBorder(w, [](size_t i, size_t j) {
            return 0.0l;
        });

        for (size_t i = 1; i < Uk->m_height - 1; i++) {
            for (size_t j = 1; j < Uk->m_width - 1; j++) {
                const value_t c1 = k1 * v(p, i - 0.5, j);
                const value_t c2 = k2 * v(q, i, j - 0.5);

                w(i, j) = (c1 * w(i - 1, j) + c2 * w(i, j - 1) + Phi(i, j)) / (1.0 + c1 + c2);
            }
        }

        for (size_t i = Uk->m_height - 2; i > 0; i--) {
            for (size_t j = Uk->m_width - 2; j > 0; j--) {
                const value_t c1 = k1 * v(p, i + 0.5, j);
                const value_t c2 = k2 * v(q, i, j + 0.5);

                w(i, j) = (c1 * w(i + 1, j) + c2 * w(i, j + 1) + w(i, j)) / (1.0 + c1 + c2);
            }
        }

        using std::numbers::pi;
        value_t tau = 2.0l / (gamma1 + gamma2 + (gamma2 - gamma1) * std::cos(pi * (2.0l * k - 1.0l) / (2.0l * it_count)));
        *Uk1 = *Uk + tau * w;

        value_t F_AUk = norm(F + Lu(*Uk1));
        value_t Uk_Ue = norm(*Uk1 - Uexact);
        value_t Uk_Uk1 = norm(*Uk1 - *Uk);
        io::printRow(6, {static_cast<value_t>(k), F_AUk, F_AUk / F_AU0, Uk_Ue, Uk_Ue / U0_Ue, Uk_Uk1});

        std::swap(Uk, Uk1);
    }
    io::endTable(6);

    printTwoSolutions(Uexact, *Uk);
}

void alternatingDirections(const math::Matrix &U0, const math::Matrix &Uexact, const math::Matrix &F)
{
    using std::numbers::sqrt2;
    const int it_estimate = std::log(2.0l / epsilon) / (2.0l * sqrt2 * std::pow(xi, 1.0l / 4.0l));
    const int it_count = 10;

    std::cout << ">>> Alternating directions" << std::endl;
    std::cout << "Iteration count estimate for epsilon=" << epsilon << ":  " << it_estimate << std::endl;

    math::Matrix _U0(U0);
    math::Matrix _U1(U0);
    math::Matrix Uk12(U0);
    math::Matrix *Uk = &_U0;
    math::Matrix *Uk1 = &_U1;

    value_t F_AU0 = norm(F + Lu(*Uk));
    value_t U0_Ue = norm(*Uk - Uexact);

    value_t tau = 2.0l / std::sqrt(delta * Delta);
    io::startTable(6, {"k", "||F-AUk||", "rel.d.", "||Uk-u*||", "rel.err.", "||Uk-Uk-1||"});
    for (int k = 1; k <= it_count; k++) {
        for (size_t j = 1; j < M; j++) {
            std::vector<value_t> S(N + 1, 0);
            std::vector<value_t> T(N + 1, 0);
            T[0] = U0(0, j);
            T[N] = U0(N, j);
            for (size_t i = 1; i < N; i++) {
                value_t Ai = v(p, i - 0.5, j) * tau / (2.0l * hx * hx);
                value_t Ci = v(p, i + 0.5, j) * tau / (2.0l * hx * hx);
                value_t Bi = Ai + Ci + 1;
                value_t Gi = -(*Uk)(i, j) - tau / 2.0l *
                                                (v(q, i, j + 0.5) * ((*Uk)(i, j + 1) - (*Uk)(i, j)) / (hy * hy) -
                                                 v(q, i, j - 0.5) * ((*Uk)(i, j) - (*Uk)(i, j - 1)) / (hy * hy) + F(i, j));
                value_t denom = (Bi - Ai * S[i - 1]);
                S[i] = Ci / denom;
                T[i] = (Ai * T[i - 1] - Gi) / denom;
            }

            for (size_t i = N - 1; i > 0; i--) {
                Uk12(i, j) = S[i] * Uk12(i + 1, j) + T[i];
            }
        }

        // std::cout << Uk12 << std::endl;

        for (size_t i = 1; i < N; i++) {
            std::vector<value_t> S(M + 1, 0);
            std::vector<value_t> T(M + 1, 0);
            T[0] = U0(i, 0);
            T[M] = U0(i, M);
            for (size_t j = 1; j < M; j++) {
                value_t Ai = v(q, i, j - 0.5) * tau / (2.0l * hy * hy);
                value_t Ci = v(q, i, j + 0.5) * tau / (2.0l * hy * hy);
                value_t Bi = Ai + Ci + 1;
                value_t Gi = -Uk12(i, j) - tau / 2.0 *
                                               (v(p, i + 0.5, j) * (Uk12(i + 1, j) - Uk12(i, j)) / (hx * hx) -
                                                v(p, i - 0.5, j) * (Uk12(i, j) - Uk12(i - 1, j)) / (hx * hx) + F(i, j));
                value_t denom = (Bi - Ai * S[i - 1]);
                S[j] = Ci / denom;
                T[j] = (Ai * T[j - 1] - Gi) / denom;
            }

            for (size_t j = M - 1; j > 0; j--) {
                (*Uk1)(i, j) = S[j] * (*Uk1)(i, j + 1) + T[j];
            }
        }

        value_t F_AUk = norm(F + Lu(*Uk1));
        value_t Uk_Ue = norm(*Uk1 - Uexact);
        value_t Uk_Uk1 = norm(*Uk1 - *Uk);
        io::printRow(6, {static_cast<value_t>(k), F_AUk, F_AUk / F_AU0, Uk_Ue, Uk_Ue / U0_Ue, Uk_Uk1});

        std::swap(Uk, Uk1);
    }
    io::endTable(6);

    printTwoSolutions(Uexact, *Uk);
}
