#pragma once
#include <cmath>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace io
{
    void printTable(
        std::vector<std::pair<long double, long double>> table,
        std::string header1,
        std::string header2,
        int indent = 0,
        int count = std::numeric_limits<int>::max()
    );
    void printTable(std::vector<std::vector<long double>> table, std::vector<std::string> headers, int indent = 0);

    void printTable(
        int width,
        int height,
        std::vector<std::string> headers,
        std::function<long double(int row, int col)> getElement,
        int indent = 0
    );

    void printArrayOfVariables(const std::vector<long double> &coefs, int startFrom = 1, std::string variable = "x");
    void printNodesAndCoefs(
        const std::vector<std::pair<long double, long double>> &coefs,
        int startFrom = 1,
        std::string variable = "x",
        std::string coef = "A"
    );

} // namespace io