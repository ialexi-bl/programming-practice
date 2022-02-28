#include "io.hpp"

static std::string tableBorder = "────────────────────────────────────────";
static int borderWidth = tableBorder.length() / 3;

namespace io
{

static void indent(int width)
{
    for (int i = 0; i < width; i++)
        std::cout << " ";
}

void printTable(std::vector<std::pair<long double, long double>> table,
                std::string header1, std::string header2, int indentWidth)
{
    std::cout << std::left;

    indent(indentWidth);
    std::cout << "┌" << tableBorder << "┬" << tableBorder << "┐" << std::endl;
    indent(indentWidth);
    std::cout << "│" << std::setw(borderWidth) << header1 << "│"
              << std::setw(borderWidth) << header2 << "│" << std::endl;

    for (auto &&it : table) {
        indent(indentWidth);
        std::cout << "├" << tableBorder << "┼" << tableBorder << "┤"
                  << std::endl;

        indent(indentWidth);
        std::cout << "│" << std::setw(borderWidth) << it.first << "│"
                  << std::setw(borderWidth) << it.second << "│" << std::endl;
    }
    indent(indentWidth);
    std::cout << "└" << tableBorder << "┴" << tableBorder << "┘" << std::endl;
}

} // namespace io