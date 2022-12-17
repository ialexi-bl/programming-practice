#pragma once
#include <functional>
#include <utility>
#include <vector>

namespace roots
{
    std::vector<std::pair<long double, long double>>
    getIntervalsWithRoots(std::function<long double(long double)> f, long double a, long double b, int N = 10);

    namespace findRootUsing
    {

        long double bisection(
            std::function<long double(long double)> f,
            long double a,
            long double b,
            long double epsilon,
            bool printDebugInfo = false
        );
        long double newton(
            std::function<long double(long double)> f,
            long double (*f_prime)(long double),
            long double (*f_prime_prime)(long double),
            long double a,
            long double b,
            long double epsilon,
            bool printDebugInfo = false
        );
        long double newtonModified(
            std::function<long double(long double)> f,
            long double (*f_prime)(long double),
            long double (*f_prime_prime)(long double),
            long double a,
            long double b,
            long double epsilon,
            bool printDebugInfo = false
        );
        long double tangents(
            std::function<long double(long double)> f,
            long double a,
            long double b,
            long double epsilon,
            bool printDebugInfo = false
        );

    } // namespace findRootUsing

    namespace findAllRootsUsing
    {
        std::vector<long double>
        tangents(std::function<long double(long double)> f, long double a, long double b, long double epsilon, int N = 10);
    }

} // namespace roots