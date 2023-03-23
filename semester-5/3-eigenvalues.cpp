#include "../lib/matrix.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <utility>

static constexpr long double EPSILON = 1e-6;

math::matrix_value estimateError(math::Matrix A, math::vector Y, math::matrix_value l)
{
    return euclideanNorm(A * Y - l * Y) / euclideanNorm(Y);
}

std::tuple<math::matrix_value, math::vector, int> getPowerEigenvector(const math::Matrix &A, long double e)
{
    math::vector Y(A.m_height);
    math::matrix_value l = 0;

    for (size_t i = 0; i < A.m_height; i++) {
        Y[i] = A.m_height * 2 - i;
    }

    int steps = 0;
    do {
        steps++;
        auto y0 = Y[0];
        Y = A * Y;
        l = Y[0] / y0;
    } while (estimateError(A, Y, l) > e);

    return {l, Y / euclideanNorm(Y), steps};
}

std::tuple<math::matrix_value, math::vector, int> getScalarEigenvector(const math::Matrix &A, long double e)
{
    math::vector _Y1(A.m_height);
    math::vector _Y2(A.m_height);

    math::vector *Y1 = &_Y1;
    math::vector *Y2 = &_Y2;
    math::matrix_value l = 0;

    for (size_t i = 0; i < A.m_height; i++) {
        (*Y1)[i] = A.m_height * 2 - i;
    }

    int steps = 0;
    do {
        steps++;
        *Y2 = A * (*Y1);
        l = ((*Y2) * (*Y1)) / ((*Y1) * (*Y1));
        std::swap(Y1, Y2);
    } while (estimateError(A, *Y1, l) > e);

    return {l, *Y1 / euclideanNorm(*Y1), steps};
}

std::tuple<math::matrix_value, math::vector, int>
getOppositeEigenvalue(const math::Matrix &A, math::matrix_value l, long double e)
{
    auto [lb, v, s] = getScalarEigenvector(A - l * math::Matrix::id(A.m_height), e);
    return {lb + l, v, s};
}

math::matrix_value aitken(math::matrix_value l2, math::matrix_value l1, math::matrix_value l)
{
    return (l * l2 - l1 * l1) / (l - 2 * l1 + l2);
}

std::tuple<math::matrix_value, math::vector, int> getWielandtEigenvalue(const math::Matrix &A, long double e)
{
    auto E = math::Matrix::id(A.m_height);
    math::vector _Y1(A.m_height, 1);
    math::vector _Y2(A.m_height, 1);

    math::matrix_value l1 = -2.55;
    math::matrix_value l2 = -2.56;
    math::vector *Y1 = &_Y1;
    math::vector *Y2 = &_Y2;

    std::vector<math::matrix_value> history {l2};
    int steps = 0;

    while (std::abs(l1 - l2) > EPSILON) {
        steps++;
        std::swap(Y1, Y2);
        std::swap(l1, l2);

        auto W = A - l1 * E;
        *Y2 = math::solveLinearSystem(W, *Y1);

        auto m = ((*Y2) * (*Y1)) / ((*Y1) * (*Y1));
        l2 = l1 + 1.0 / m;
        history.push_back(l2);
    };

    if (int historyLength = history.size(); historyLength >= 3) {
        l2 = aitken(history[historyLength - 3], history[historyLength - 2], history[historyLength - 1]);
    }

    return {l2, (*Y2) / euclideanNorm(*Y2), steps};
}

int main()
{
    std::cout << std::setprecision(30);
    math::Matrix m(3, 3, {{-1.48213, -0.03916, 1.08254}, {-0.03916, 1.13958, 0.01617}, {1.08254, 0.01617, -1.48271}});

    std::cout << "Matrix:" << std::endl << m << std::endl;

    size_t jacobiSteps;
    auto jacobiResult = getJacobiEigenvectors(m, EPSILON, &jacobiSteps);
    std::cout << "Result by Jacobi in " << jacobiSteps << " steps:" << std::endl;
    for (size_t i = 0; i < jacobiResult.size(); i++) {
        std::cout << "> Eigenvalue " << jacobiResult[i].first << ", eigenvector " << jacobiResult[i].second << std::endl;
    }
    std::cout << std::endl;

    auto [powerValue, powerVector, powerSteps] = getPowerEigenvector(m, EPSILON);
    std::cout << "Result by powers:" << std::endl
              << "> Eigenvalue " << powerValue << ", eigenvector " << powerVector << " in " << powerSteps << " steps"
              << std::endl;

    auto [scalarValue, scalarVector, scalarSteps] = getScalarEigenvector(m, EPSILON * EPSILON);
    std::cout << "Result by scalar product:" << std::endl
              << "> Eigenvalue " << scalarValue << ", eigenvector " << scalarVector << " in " << scalarSteps << " steps"
              << std::endl;

    auto [oppositeValue, oppositeVector, oppositeSteps] = getOppositeEigenvalue(m, scalarValue, EPSILON);
    std::cout << "Opposite eigenvalue:" << std::endl
              << "> Eigenvalue " << oppositeValue << ", eigenvector " << oppositeVector << " in " << oppositeSteps << " steps"
              << std::endl;

    auto [wielandtValue, wielandtVector, wielandtSteps] = getWielandtEigenvalue(m, EPSILON);
    std::cout << "Wielandt eigenvalue:" << std::endl
              << "> Eigenvalue " << wielandtValue << ", eigenvector " << wielandtVector << " in " << wielandtSteps << " steps"
              << std::endl;
}