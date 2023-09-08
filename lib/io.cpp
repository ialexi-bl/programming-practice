#include "io.hpp"
#include <algorithm>
#include <cassert>

namespace io
{

    static void indent(size_t width)
    {
        for (size_t i = 0; i < width; i++) std::cout << ' ';
    }

    static std::string getBorder(size_t cell_width)
    {
        std::string table_border = "";
        for (size_t i = 0; i < cell_width; i++) {
            table_border += "─";
        }
        return table_border;
    }

    void startTable(size_t width, std::vector<std::string> headers, size_t cell_width, size_t indent_width)
    {
        std::cout << std::right;

        std::string cell = getBorder(cell_width);
        indent(indent_width);
        std::cout << "┌";
        for (size_t i = 0; i < width; i++) {
            if (i) std::cout << "┬";
            std::cout << cell;
        }
        std::cout << "┐" << std::endl;

        indent(indent_width);
        for (size_t i = 0; i < width; i++) {
            std::cout << "│" << std::setw(cell_width) << headers[i];
        }
        std::cout << "│" << std::endl << std::left;
    }
    static void printRowDivider(size_t width, size_t cell_width, size_t indent_width)
    {
        std::string cell = getBorder(cell_width);

        indent(indent_width);
        std::cout << "├";
        for (size_t j = 0; j < width; j++) {
            if (j) std::cout << "┼";
            std::cout << cell;
        }
        std::cout << "┤" << std::endl;
    }
    void endTable(size_t width, size_t cell_width, size_t indent_width)
    {
        std::string cell = getBorder(cell_width);

        indent(indent_width);
        std::cout << "└";
        for (size_t j = 0; j < width; j++) {
            if (j) std::cout << "┴";
            std::cout << cell;
        }
        std::cout << "┘" << std::endl;
    }

    void printRow(size_t width, std::vector<long double> values, size_t cell_width, size_t indent_width)
    {
        printRowDivider(width, cell_width, indent_width);

        indent(indent_width);
        for (size_t i = 0; i < width; i++) {
            std::cout << "│" << std::setw(cell_width) << values[i];
        }
        std::cout << "│" << std::endl << std::left;
    }

    void printTable(
        std::vector<std::pair<long double, long double>> table,
        std::string header1,
        std::string header2,
        size_t indent_width,
        size_t count
    )
    {
        return printTable(
            2,
            std::min<int>(table.size(), count),
            DEFAULT_CELL_WIDTH,
            {header1, header2},
            [&table](size_t row, size_t col) {
                return col ? table[row].second : table[row].first;
            },
            indent_width
        );
    }

    void printTable(std::vector<std::vector<long double>> table, std::vector<std::string> headers, size_t indent_width)
    {

        size_t width = headers.size(), height = table.size();
        for (auto &&row : table) {
            assert(row.size() == width || "Invalid table");
        }

        printTable(
            width,
            height,
            DEFAULT_CELL_WIDTH,
            headers,
            [&table](size_t row, size_t col) {
                return table[row][col];
            },
            indent_width
        );
    }

    void printTable(
        size_t width,
        size_t height,
        std::vector<std::string> headers,
        std::function<long double(size_t, size_t)> getElement
    )
    {
        printTable(width, height, DEFAULT_CELL_WIDTH, headers, getElement);
    }

    void printTable(
        size_t width,
        size_t height,
        size_t cell_width,
        std::vector<std::string> headers,
        std::function<long double(size_t row, size_t col)> getElement,
        size_t indent_width
    )
    {
        startTable(width, headers, cell_width, indent_width);
        for (size_t i = 0; i < height; i++) {
            std::vector<long double> row;
            for (size_t j = 0; j < width; j++) {
                row.push_back(getElement(i, j));
            }

            printRow(width, row, cell_width, indent_width);
        }
        endTable(width, cell_width, indent_width);
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