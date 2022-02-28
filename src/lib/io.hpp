#include <iomanip>
#include <iostream>
#include <utility>
#include <vector>

namespace io
{

void printTable(std::vector<std::pair<long double, long double>> table,
                std::string header1, std::string header2, int indent = 0);

}