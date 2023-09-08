#include "../lib/io.hpp"
#include "../lib/math.hpp"
#include "../lib/matrix.hpp"
#include <assert.h>
#include <cmath>
#include <iostream>

using math::simple_function_t;
using math::value_t;

value_t a11 = -125.0;
value_t a12 = 123.15;
value_t a21 = 123.15;
value_t a22 = -123.0;

value_t t0 = 0.0;
value_t _y1 = 1.0;
value_t _y2 = 1.0;

std::function<math::vector(value_t)> solveExact(const math::Matrix &M, value_t t0, const math::vector &y0)
{
    size_t n = M.m_height;
    assert(M.isDiagonal() || "Only diagonal matrices are supported");

    auto eigenvectors = math::getJacobiEigenvectors(M, 1e-20);
    math::Matrix A(n, n);

    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            auto &[eigenvalue, eigenvector] = eigenvectors[j];
            A(i, j) = eigenvector[i] * std::exp(eigenvalue * t0);
        }
    }

    auto C = math::solveLinearSystem(A, y0);
    return [=](value_t t) {
        math::vector result;
        for (size_t i = 0; i < n; i++) {
            value_t r = 0;

            for (size_t j = 0; j < n; j++) {
                auto &[eigenvalue, eigenvector] = eigenvectors[j];
                r += C[j] * eigenvector[i] * std::exp(eigenvalue * t);
            }

            result.push_back(r);
        }

        return result;
    };
}

std::vector<math::vector> solveEulerExplicit(const math::Matrix &A, const math::vector &Y0, value_t h, size_t iterations)
{
    size_t n = A.m_height;
    assert(n == A.m_width || "Only square matrices are supported");

    auto W = math::Matrix::id(n) + h * A;
    std::vector<math::vector> result = {Y0};
    for (size_t i = 0; i < iterations; i++) {
        result.push_back(W * result[i]);
    }
    return result;
}

std::vector<math::vector> solveEulerImplicit(const math::Matrix &A, const math::vector &Y0, value_t h, size_t iterations)
{
    size_t n = A.m_height;
    assert(n == A.m_width || "Only square matrices are supported");

    auto W = (math::Matrix::id(n) - h * A).invert();
    std::vector<math::vector> result = {Y0};
    for (size_t i = 0; i < iterations; i++) {
        result.push_back(W * result[i]);
    }
    return result;
}

std::vector<math::vector> solveAdams(const math::Matrix &A, const math::vector &Y0, value_t h, size_t iterations)
{
    size_t n = A.m_height;
    assert(n == A.m_width || "Only square matrices are supported");

    auto E = math::Matrix::id(n);
    auto W = (E - (5.0 * h / 12.0) * A).invert();
    auto W1 = W * (E + (2.0 * h / 3.0) * A);
    auto W2 = W * ((-h / 12.0) * A);

    std::vector<math::vector> result = {Y0, (E - h * A).invert() * Y0};
    for (size_t i = 1; i < iterations; i++) {
        result.push_back(W1 * result[i] + W2 * result[i - 1]);
    }
    return result;
}

int main()
{
    math::Matrix A = {{{a11, a12}, {a21, a22}}};
    math::vector Y0 = {_y1, _y2};

    constexpr value_t h = 0.05;
    constexpr size_t cell_width = 15;
    // std::cout << std::setprecision(16);

    auto exactSolution = solveExact(A, t0, Y0);
    auto euler_explicit_solution = solveEulerExplicit(A, Y0, h, std::round(0.5 / h));
    auto euler_implicit_solution = solveEulerImplicit(A, Y0, h, std::round(0.5 / h));
    auto adams_solution = solveAdams(A, Y0, h, std::round(0.5 / h));

    io::printTable(
        9,
        6,
        cell_width,
        {"t", "exact y1", "exact y2", "explicit y1", "explicit y2", "implicit y1", "implicit y2", "adams y1", "adams y2"},
        [&](size_t row, size_t col) -> value_t {
            value_t t = 0.1 * row;
            size_t i = row * (0.1 / h);
            switch (col) {
            case 0:
                return t;
            case 1:
                return exactSolution(t)[0];
            case 2:
                return exactSolution(t)[1];
            case 3:
                return euler_explicit_solution[i][0];
            case 4:
                return euler_explicit_solution[i][1];
            case 5:
                return euler_implicit_solution[i][0];
            case 6:
                return euler_implicit_solution[i][1];
            case 7:
                return adams_solution[i][0];
            case 8:
                return adams_solution[i][1];
            default:
                return 0.0;
            }
        }
    );

    euler_explicit_solution = solveEulerExplicit(A, Y0, 0.001, 0.5 / 0.001);
    euler_implicit_solution = solveEulerImplicit(A, Y0, 0.001, 0.5 / 0.001);
    adams_solution = solveAdams(A, Y0, 0.001, 0.5 / 0.001);

    io::printTable(
        9,
        6,
        cell_width,
        {"t", "exact y1", "exact y2", "explicit y1", "explicit y2", "implicit y1", "implicit y2", "adams y1", "adams y2"},
        [&](size_t row, size_t col) -> value_t {
            value_t t = 0.1 * row;
            size_t i = row * (0.1 / 0.001);
            switch (col) {
            case 0:
                return t;
            case 1:
                return exactSolution(t)[0];
            case 2:
                return exactSolution(t)[1];
            case 3:
                return euler_explicit_solution[i][0];
            case 4:
                return euler_explicit_solution[i][1];
            case 5:
                return euler_implicit_solution[i][0];
            case 6:
                return euler_implicit_solution[i][1];
            case 7:
                return adams_solution[i][0];
            case 8:
                return adams_solution[i][1];
            default:
                return 0.0;
            }
        }
    );
}
