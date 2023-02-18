#include "../lib/matrix.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <utility>

static constexpr double EPSILON = 1e-3;

static math::matrix_value vectorNorm(const math::Matrix &m)
{
    math::matrix_value result = 0;
    for (int i = 0; i < m.m_values.size(); i++) {
        result += std::abs(m.m_values[i]);
    }
    return result;
}

static std::pair<math::Matrix, math::Matrix> transformSystem(const math::Matrix &A, const math::vector &v)
{
    if (A.m_height != A.m_width || A.m_height != v.size()) {
        throw std::runtime_error("Unable to transform a system with such dimensions");
    }

    math::Matrix E = math::Matrix::id(A.m_height);
    math::Matrix Di(A.m_height, A.m_width);
    for (int i = 0; i < A.m_height; i++) {
        Di(i, i) = 1 / A(i, i);
    }

    math::Matrix H = E - Di * A;
    math::Matrix g = Di * math::Matrix(v.size(), 1, v);

    return {H, g};
}

static int getStepsEstimate(const math::Matrix &H, const math::Matrix &g)
{
    auto norm = H.norm();
    auto norm_g = vectorNorm(g);
    int k = 0;
    double estimate = std::numeric_limits<double>::infinity();
    while (std::abs(estimate) > EPSILON) {
        k++;
        estimate = ((std::pow(norm, k) / (1 - norm)) * norm_g);
    }
    return k;
}

static math::matrix_value error(const math::vector &solution, const math::vector &realSolution)
{
    math::matrix_value error = 0;

    for (int i = 0; i < solution.size(); i++) {
        error += std::abs(realSolution[i] - solution[i]);
    }

    return error;
}

static math::vector
simpleIteration(const math::Matrix &H, const math::Matrix &g, const math::vector &realSolution, int *steps = nullptr)
{
    math::Matrix result = g;

    int k = -1;
    while (++k, error(result.m_values, realSolution) > EPSILON) {
        result = H * result + g;
    }

    if (steps) {
        *steps = k;
    }

    return result.m_values;
}

static math::vector seidel(const math::Matrix &H, const math::Matrix &g, const math::vector &realSolution, int *steps = nullptr)
{
    auto result = g.m_values;

    int k = -1;
    while (++k, error(result, realSolution) > EPSILON) {
        std::vector<math::matrix_value> next(g.m_height);

        for (int i = 0; i < g.m_height; i++) {
            next[i] = g(i, 0);
            for (int j = 0; j < i; j++) {
                next[i] += H(i, j) * next[j];
            }
            for (int j = i; j < g.m_height; j++) {
                next[i] += H(i, j) * result[j];
            }
        }

        result = next;
    }

    if (steps) {
        *steps = k;
    }

    return result;
}

static math::vector
relaxation(const math::Matrix &H, const math::Matrix &g, const math::vector &realSolution, int *steps = nullptr)
{
    math::matrix_value spectralRadius = H.spectralRadius();
    math::matrix_value q = 2 / (1 + std::sqrt(1 - spectralRadius * spectralRadius));

    auto result = g.m_values;

    int k = -1;
    while (++k, error(result, realSolution) > EPSILON) {
        std::vector<math::matrix_value> next(g.m_height);

        for (int i = 0; i < g.m_height; i++) {
            next[i] = g(i, 0) - result[i];
            for (int j = 0; j < i; j++) {
                next[i] += H(i, j) * next[j];
            }
            for (int j = i + 1; j < g.m_height; j++) {
                next[i] += H(i, j) * result[j];
            }

            next[i] *= q;
            next[i] += result[i];
        }

        result = next;
    }

    if (steps) {
        *steps = k;
    }

    return result;
}

int main()
{
    std::cout << std::setprecision(10);
    math::Matrix A(3, 3, {{1, 0, 0.9}, {4, 6, 1}, {2, 1, 5}});
    math::vector b = {17, 24, 28};

    auto transformed = transformSystem(A, b);
    auto H = transformed.first;
    auto g = transformed.second;

    std::cout << "Transformed system:" << std::endl
              << "> H:" << std::endl
              << H << std::endl
              << "> g:" << std::endl
              << g.m_values << std::endl
              << std::endl;

    math::matrix_value norm = H.norm();
    std::cout << "||H|| = " << norm << std::endl;

    int k = getStepsEstimate(H, g);
    std::cout << "Estimate: k = " << k << std::endl;

    int iterativeSteps, seidelSteps, relaxSteps;

    auto realSolution = math::solveLinearSystem(A, b);
    std::cout << "Real solution:       " << realSolution << std::endl;
    auto iterativeSolution = simpleIteration(H, g, realSolution, &iterativeSteps);
    std::cout << "Iteration solution:  " << iterativeSolution << ", calculated in " << iterativeSteps << " steps" << std::endl;
    auto seidelSolution = seidel(H, g, realSolution, &seidelSteps);
    std::cout << "Seidel solution:     " << seidelSolution << ", calculated in " << seidelSteps << " steps" << std::endl;
    auto relaxSolution = relaxation(H, g, realSolution, &relaxSteps);
    std::cout << "Relaxation solution: " << relaxSolution << ", calculated in " << relaxSteps << " steps" << std::endl;
}