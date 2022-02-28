#include <utility>
#include <vector>

namespace polynomials
{

using InterpolationTable = std::vector<std::pair<long double, long double>>;

InterpolationTable getInterpolationTable(long double (*f)(long double),
                                         long double m, long double a,
                                         long double b);
void sortInterpolationTable(InterpolationTable &table, long double x);

namespace interpolateUsing
{

long double lagrange(const InterpolationTable &table, int n, long double x);
long double newton(const InterpolationTable &table, int n, long double x);

} // namespace interpolateUsing

} // namespace polynomials
