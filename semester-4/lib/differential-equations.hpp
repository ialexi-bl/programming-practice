#include <cmath>
#include <vector>

namespace diff
{
    using FirstOrderEquation = std::function<long double(long double, long double)>;

    namespace continueTableUsing
    {
        std::vector<long double>
        adams(const std::vector<long double> &table, FirstOrderEquation f, long double xn, long double h, int length);
        std::vector<long double>
        rungeKutta(const std::vector<long double> &table, FirstOrderEquation f, long double xn, long double h, int length);
        std::vector<long double>
        euler(const std::vector<long double> &table, FirstOrderEquation f, long double xn, long double h, int length);
        std::vector<long double>
        euler1(const std::vector<long double> &table, FirstOrderEquation f, long double xn, long double h, int length);
        std::vector<long double>
        euler2(const std::vector<long double> &table, FirstOrderEquation f, long double xn, long double h, int length);
    } // namespace continueTableUsing
} // namespace diff