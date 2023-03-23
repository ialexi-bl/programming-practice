#include <cmath>
#include <functional>
#include <vector>

namespace diff
{
    using diff_operator_t = std::function<long double(long double, long double)>;

    namespace continueTableUsing
    {
        std::vector<long double>
        adams(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length);
        std::vector<long double>
        rungeKutta(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length);
        std::vector<long double>
        euler(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length);
        std::vector<long double>
        euler1(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length);
        std::vector<long double>
        euler2(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length);
    } // namespace continueTableUsing
} // namespace diff