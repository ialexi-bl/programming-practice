#pragma once
#include <array>
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
    constexpr size_t DEFAULT_CELL_WIDTH = 15;

    void
    startTable(size_t width, std::vector<std::string> headers, size_t cell_width = DEFAULT_CELL_WIDTH, size_t indent_width = 0);
    void printRow(size_t width, std::vector<long double> values, size_t cell_width = DEFAULT_CELL_WIDTH, size_t indent_width = 0);
    void endTable(size_t width, size_t cell_width = DEFAULT_CELL_WIDTH, size_t indent_width = 0);

    void printTable(
        std::vector<std::pair<long double, long double>> table,
        std::string header1,
        std::string header2,
        size_t indent = 0,
        size_t count = std::numeric_limits<int>::max()
    );
    void printTable(std::vector<std::vector<long double>> table, std::vector<std::string> headers, size_t indent = 0);

    void printTable(
        size_t width,
        size_t height,
        std::vector<std::string> headers,
        std::function<long double(size_t row, size_t col)> get_element
    );
    void printTable(
        size_t width,
        size_t height,
        size_t cell_width,
        std::vector<std::string> headers,
        std::function<long double(size_t row, size_t col)> get_element,
        size_t indent = 0
    );

    void printFunction(
        std::function<long double(long double)> f,
        const std::string &name,
        long double from,
        long double to,
        long double step
    );

    void printArrayOfVariables(const std::vector<long double> &coefs, size_t start_from = 1, std::string variable = "x");
    void printNodesAndCoefs(
        const std::vector<std::pair<long double, long double>> &coefs,
        size_t start_from = 1,
        std::string variable = "x",
        std::string coef = "A"
    );

} // namespace io