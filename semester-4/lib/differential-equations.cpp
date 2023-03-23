#include "differential-equations.hpp"
#include <assert.h>
#include <functional>
#include <vector>

namespace diff
{

    static std::vector<long double> calculateRightmostDifferences(const std::vector<long double> &values)
    {
        std::vector<long double> result = values;

        for (int i = 1, s = result.size(); i < s; i++) {
            for (int j = 0; j < s - i; j++) {
                result[j] = result[j + 1] - result[j];
            }
        }

        return result;
    }

    static void shiftRightmostDifferences(std::vector<long double> &values, long double value)
    {
        size_t s = values.size();

        long double saved = values[s - 1];
        values[s - 1] = value;

        for (int i = 2, s = values.size(); i <= s; i++) {
            long double nextValue = values[s - i + 1] - saved;
            saved = values[s - i];
            values[s - i] = nextValue;
        }
    }

    namespace continueTableUsing
    {
        std::vector<long double>
        adams(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length)
        {
            assert(table.size() == 5 || "4th order Adams' method requires exactly 5 nodes");

            std::vector<long double> result = table;

            std::vector<long double> derivatives(5);
            std::generate(derivatives.begin(), derivatives.end(), [xn, h, &f, &table, m = -4]() mutable {
                long double x = xn + m * h;
                return f(x, table[4 + m++]);
            });

            std::vector<long double> differences = calculateRightmostDifferences(derivatives);

            for (int s = result.size(), i = s - 1; i < length; i++) {
                long double value = result[i] + h * (differences[4] + (differences[3] / 2) + (5 * differences[2] / 12) +
                                                     (3 * differences[1] / 8) + (251 * differences[0] / 720));
                result.push_back(value);
                shiftRightmostDifferences(differences, f(xn + (i - (s - 1) + 1) * h, value));
            }

            return result;
        }

        std::vector<long double>
        rungeKutta(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length)
        {
            std::vector<long double> result = table;

            for (int s = result.size(), i = s - 1; i < length; i++) {
                long double x = xn + (i - (s - 1)) * h;
                long double y = result[i];
                long double k1 = h * f(x, y);
                long double k2 = h * f(x + h / 2, y + k1 / 2);
                long double k3 = h * f(x + h / 2, y + k2 / 2);
                long double k4 = h * f(x + h, y + k3);

                result.push_back(y + (k1 + 2 * k2 + 2 * k3 + k4) / 6);
            }

            return result;
        }

        std::vector<long double>
        euler(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length)
        {
            std::vector<long double> result = table;
            for (int s = result.size(), i = s - 1; i < length; i++) {
                long double x = xn + (i - (s - 1)) * h;
                result.push_back(result[i] + h * f(x, result[i]));
            }

            return result;
        }

        std::vector<long double>
        euler1(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length)
        {
            std::vector<long double> result = table;
            for (int s = result.size(), i = s - 1; i < length; i++) {
                long double x = xn + (i - (s - 1)) * h;
                long double y = result[i];
                long double y12 = y + h * f(x, y) / 2;

                result.push_back(y + h * f(x + h / 2, y12));
            }

            return result;
        }

        std::vector<long double>
        euler2(const std::vector<long double> &table, diff_operator_t f, long double xn, long double h, int length)
        {
            std::vector<long double> result = table;
            for (int s = result.size(), i = s - 1; i < length; i++) {
                long double x = xn + (i - (s - 1)) * h;
                long double y = result[i];
                long double y1 = y + h * f(x, y);

                result.push_back(y + h * (f(x, y) + f(x, y1)) / 2);
            }

            return result;
        }
    } // namespace continueTableUsing

} // namespace diff