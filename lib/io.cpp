#include "io.hpp"
#include <algorithm>
#include <cassert>

namespace io
{
    static std::string table_border = "─────────────────────────";
    static size_t border_width = table_border.length() / 3;

    static void indent(size_t width)
    {
        for (size_t i = 0; i < width; i++) std::cout << ' ';
    }

    void printTable(
        std::vector<std::pair<long double, long double>> table,
        std::string header1,
        std::string header2,
        size_t indentWidth,
        size_t count
    )
    {
        return printTable(
            2,
            std::min<int>(table.size(), count),
            {header1, header2},
            [&table](size_t row, size_t col) {
                return col ? table[row].second : table[row].first;
            },
            indentWidth
        );
    }

    void printTable(std::vector<std::vector<long double>> table, std::vector<std::string> headers, size_t indentWidth)
    {

        size_t width = headers.size(), height = table.size();
        for (auto &&row : table) {
            assert(row.size() == width || "Invalid table");
        }

        printTable(
            width,
            height,
            headers,
            [&table](size_t row, size_t col) {
                return table[row][col];
            },
            indentWidth
        );
    }

    void printTable(
        size_t width,
        size_t height,
        std::vector<std::string> headers,
        std::function<long double(size_t, size_t)> getElement,
        size_t indentWidth
    )
    {
        std::cout << std::right;

        indent(indentWidth);
        std::cout << "┌";
        for (size_t i = 0; i < width; i++) {
            if (i) std::cout << "┬";
            std::cout << table_border;
        }
        std::cout << "┐" << std::endl;

        indent(indentWidth);
        for (size_t i = 0; i < width; i++) {
            std::cout << "│" << std::setw(border_width) << headers[i];
        }
        std::cout << "│" << std::endl << std::left;

        for (size_t i = 0; i < height; i++) {
            indent(indentWidth);
            std::cout << "├";
            for (size_t j = 0; j < width; j++) {
                if (j) std::cout << "┼";
                std::cout << table_border;
            }
            std::cout << "┤" << std::endl;

            indent(indentWidth);
            for (size_t j = 0; j < width; j++) {
                std::cout << "│" << std::setw(border_width) << getElement(i, j);
            }
            std::cout << "│" << std::endl;
        }

        indent(indentWidth);
        std::cout << "└";
        for (size_t j = 0; j < width; j++) {
            if (j) std::cout << "┴";
            std::cout << table_border;
        }
        std::cout << "┘" << std::endl;
    }

    void printFunction(
        std::function<long double(long double)> f,
        const std::string &name,
        long double from,
        long double to,
        long double step
    )
    {
        printTable(2, static_cast<int>((to - from) / step), {"x", name}, [=](size_t row, size_t col) {
            long double x = from + step * row;
            switch (col) {
            case 0:
                return x;
            case 1:
                return f(x);
            default:
                return nanl("impossible");
            }
        });
    }

    void printArrayOfVariables(const std::vector<long double> &coefs, size_t startFrom, std::string variable)
    {
        for (size_t i = 0, s = coefs.size(); i < s; i++) {
            std::cout << "  " << variable << (i + startFrom) << " = " << coefs[i] << std::endl;
        }
    }

    void printNodesAndCoefs(
        const std::vector<std::pair<long double, long double>> &coefs,
        size_t startFrom,
        std::string variable,
        std::string coef
    )
    {
        for (size_t i = 0, s = coefs.size(); i < s; i++) {
            std::cout << "  " << variable << (i + startFrom) << " = " << std::left << std::setw(35) << coefs[i].first << "; "
                      << coef << (i + 1) << " = " << coefs[i].second << std::endl;
        }
    }

} // namespace io