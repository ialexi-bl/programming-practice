#include "io.hpp"
#include <algorithm>
#include <cassert>

namespace io
{
    static std::string tableBorder = "─────────────────────────";
    static size_t borderWidth = tableBorder.length() / 3;

    static void indent(int width)
    {
        for (int i = 0; i < width; i++) std::cout << " ";
    }

    void printTable(
        std::vector<std::pair<long double, long double>> table,
        std::string header1,
        std::string header2,
        int indentWidth,
        int count
    )
    {
        return printTable(
            2,
            std::min<int>(table.size(), count),
            {header1, header2},
            [&table](int row, int col) {
                return col ? table[row].second : table[row].first;
            },
            indentWidth
        );
    }

    void printTable(std::vector<std::vector<long double>> table, std::vector<std::string> headers, int indentWidth)
    {

        size_t width = headers.size(), height = table.size();
        for (auto &&row : table) {
            assert(row.size() == width || "Invalid table");
        }

        printTable(
            width,
            height,
            headers,
            [&table](int row, int col) {
                return table[row][col];
            },
            indentWidth
        );
    }

    void printTable(
        int width,
        int height,
        std::vector<std::string> headers,
        std::function<long double(int, int)> getElement,
        int indentWidth
    )
    {
        std::cout << std::right;

        indent(indentWidth);
        std::cout << "┌";
        for (int i = 0; i < width; i++) {
            if (i) std::cout << "┬";
            std::cout << tableBorder;
        }
        std::cout << "┐" << std::endl;

        indent(indentWidth);
        for (int i = 0; i < width; i++) {
            std::cout << "│" << std::setw(borderWidth) << headers[i];
        }
        std::cout << "│" << std::endl << std::left;

        for (int i = 0; i < height; i++) {
            indent(indentWidth);
            std::cout << "├";
            for (int j = 0; j < width; j++) {
                if (j) std::cout << "┼";
                std::cout << tableBorder;
            }
            std::cout << "┤" << std::endl;

            indent(indentWidth);
            for (int j = 0; j < width; j++) {
                std::cout << "│" << std::setw(borderWidth) << getElement(i, j);
            }
            std::cout << "│" << std::endl;
        }

        indent(indentWidth);
        std::cout << "└";
        for (int j = 0; j < width; j++) {
            if (j) std::cout << "┴";
            std::cout << tableBorder;
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
        printTable(2, static_cast<int>((to - from) / step), {"x", name}, [=](int row, int col) {
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

    void printArrayOfVariables(const std::vector<long double> &coefs, int startFrom, std::string variable)
    {
        for (int i = 0, s = coefs.size(); i < s; i++) {
            std::cout << "  " << variable << (i + startFrom) << " = " << coefs[i] << std::endl;
        }
    }

    void printNodesAndCoefs(
        const std::vector<std::pair<long double, long double>> &coefs,
        int startFrom,
        std::string variable,
        std::string coef
    )
    {
        for (int i = 0, s = coefs.size(); i < s; i++) {
            std::cout << "  " << variable << (i + startFrom) << " = " << std::left << std::setw(35) << coefs[i].first << "; "
                      << coef << (i + 1) << " = " << coefs[i].second << std::endl;
        }
    }

} // namespace io