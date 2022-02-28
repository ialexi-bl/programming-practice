#include <utility>
#include <vector>

namespace roots
{
std::vector<std::pair<long double, long double>>
getIntervalsWithRoots(long double (*function)(long double), long double a,
                      long double b, int N = 10);

namespace findRootUsing
{

long double bisection(long double (*f)(long double), long double a,
                      long double b, long double epsilon,
                      bool printDebugInfo = false);
long double newton(long double (*f)(long double),
                   long double (*f_prime)(long double),
                   long double (*f_prime_prime)(long double), long double a,
                   long double b, long double epsilon,
                   bool printDebugInfo = false);
long double newtonModified(long double (*f)(long double),
                           long double (*f_prime)(long double),
                           long double (*f_prime_prime)(long double),
                           long double a, long double b, long double epsilon,
                           bool printDebugInfo = false);
long double tangents(long double (*f)(long double), long double a,
                     long double b, long double epsilon,
                     bool printDebugInfo = false);

} // namespace findRootUsing

} // namespace roots