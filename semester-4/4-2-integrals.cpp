#include "lib/functions.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <iterator>

namespace polynomials
{
    functions::Polynomial polynomials[] = {{{5.0l}},
                                           {{8.0l, 9.0l}},
                                           {{2.0l, -24.0l, 3.0l}},
                                           {{-15.0l, 0.0l, -30.0l, 3.0l}},
                                           {{-15.0l, 0.0l, -30.0l, 3.0l, -15.0l, 0.0l, -30.0l, 3.0l}}};
    constexpr size_t polynomials_count = std::size(polynomials);

    functions::Integrator getIntegrator(int i)
    {
        return [&, i](long double a, long double b) {
            return polynomials[i].integrate(a, b);
        };
    }

    functions::Func getEvaluator(int i)
    {
        return polynomials[i];
    }

    functions::ExtrCalculator getDerivativeAbsMaxCalculator(int i)
    {
        return polynomials[i].getDerivativeAbsMaxCalculator(1);
    }

    functions::ExtrCalculator getSecondDerivativeAbsMaxCalculator(int i)
    {
        return polynomials[i].getDerivativeAbsMaxCalculator(2);
    }

    functions::ExtrCalculator getFourthDerivativeAbsMaxCalculator(int i)
    {
        return polynomials[i].getDerivativeAbsMaxCalculator(4);
    }
} // namespace polynomials

namespace function
{
    std::string str = "5/(1+e^(-x))";

    long double antiderivative(long double x)
    {
        return 5 * std::log(std::exp(x) + 1);
    }

    long double integrate(long double a, long double b)
    {
        return antiderivative(b) - antiderivative(a);
    }

    long double evaluate(long double x)
    {
        return 5 / (1 + std::exp(-x));
    }

    long double derivative(long double x)
    {
        return 5 * std::exp(-x) / ((1 + std::exp(-x)) * (1 + std::exp(-x)));
    }

    long double second_derivative(long double x)
    {
        return 5 * ((2 * std::exp(-2 * x)) / std::pow(1 + std::exp(-x), 3) - std::exp(-x) / std::pow(1 + std::exp(-x), 2));
    }

    long double fourth_derivative(long double x)
    {
        return 5 * ((24 * std::exp(-4 * x)) / std::pow(1 + std::exp(-x), 5) -
                    (36 * std::exp(-3 * x)) / std::pow(1 + std::exp(-x), 4) +
                    (14 * std::exp(-2 * x)) / std::pow(1 + std::exp(-x), 3) - (std::exp(-x) / std::pow(1 + std::exp(-x), 2)));
    }

    // |f'| возрастает на [-∞,0) и убывает на (0,+∞]
    long double derivative_abs_max(long double a, long double b)
    {
        return functions::getAbsMaxOnInterval({0}, derivative, a, b);
    }

    // |f''| чётная, возрастает на [-∞,log(2-√3)), убывает на (log(2-√3),0)
    long double second_derivative_abs_max(long double a, long double b)
    {
        return functions::getAbsMaxOnInterval({std::log(2 - std::sqrt(3)), std::log(2 + std::sqrt(3))}, second_derivative, a, b);
    }

    long double fourth_derivative_abs_max(long double a, long double b)
    {
        return functions::getAbsMaxOnInterval(
            {std::log((1.0 / 2.0) * (13 - std::sqrt(105) - std::sqrt(2 * (135 - 13 * std::sqrt(105))))),
             std::log((1.0 / 2.0) * (13 - std::sqrt(105) + std::sqrt(2 * (135 - 13 * std::sqrt(105))))),
             std::log((1.0 / 2.0) * (13 + std::sqrt(105) - std::sqrt(2 * (135 - 13 * std::sqrt(105))))),
             std::log((1.0 / 2.0) * (13 + std::sqrt(105) + std::sqrt(2 * (135 - 13 * std::sqrt(105)))))},
            fourth_derivative, a, b);
    }
} // namespace function

namespace exponent
{
    std::string str = "e^x";

    long double integrate(long double a, long double b)
    {
        return std::exp(b) - std::exp(a);
    }

    long double evaluate(long double x)
    {
        return std::exp(x);
    }

    // |f'| возрастает на [-∞,0) и убывает на (0,+∞]
    long double derivative_abs_max(long double a, long double b)
    {
        return std::exp(b);
    }

    // |f''| чётная, возрастает на [-∞,log(2-√3)), убывает на (log(2-√3),0)
    long double second_derivative_abs_max(long double a, long double b)
    {
        return std::exp(b);
    }

    long double fourth_derivative_abs_max(long double a, long double b)
    {
        return std::exp(b);
    }
} // namespace exponent

long double applyRunge(int l, long double jh, long double jhl, int r)
{
    long long lr = std::pow(l, r);
    return (lr * jhl - jh) / (lr - 1);
}

int main()
{
    std::cout << std::setprecision(20);

    int i = 6, l = 1, m = 1000;
    long double a = 1, b = 20;
    long double realValue = 0;
    long double leftRect = 0;
    long double middleRect = 0;
    long double rightRect = 0;
    long double trapezoid = 0;
    long double simpson = 0;
    long double threeEights = 0;

    do {
        if (l == 1) {
            std::cout << "> Выберите функцию:" << std::endl;
            for (int i = 0; i < polynomials::polynomials_count; i++) {
                std::cout << (i + 1) << ". " << static_cast<std::string>(polynomials::polynomials[i]) << std::endl;
            }
            std::cout << (polynomials::polynomials_count + 1) << ". " << function::str << std::endl
                      << (polynomials::polynomials_count + 2) << ". " << exponent::str << std::endl;
            do {
                std::cout << "$ ";
                std::cin >> i;
                i--;

                if (i >= 0 && i <= polynomials::polynomials_count + 1) {
                    break;
                } else {
                    std::cout << "! Недопустимый номер функции, введите ещё раз:" << std::endl;
                }
            } while (true);

            std::cout << std::endl
                      << "> Введите пределы интегрирования (положительные числа через "
                         "пробел или перевод строки)"
                      << std::endl;
            std::cin >> a >> b;

            std::cout << std::endl << "> Введите число промежутков деления m:" << std::endl;
            std::cin >> m;
        }

        long double h = (b - a) / (m * l);
        std::cout << std::endl << "> Число промежутков деления: " << (l == 1 ? "m = " : "m * l = ") << m * l << std::endl;
        std::cout << "> Длина промежутков деления: h = " << h << std::endl;

        functions::Integrator integrate = i == polynomials::polynomials_count + 1 ? exponent ::integrate
                                          : i == polynomials::polynomials_count   ? function::integrate
                                                                                  : polynomials::getIntegrator(i);
        functions::Func evaluate = i == polynomials::polynomials_count + 1 ? exponent::evaluate
                                   : i == polynomials::polynomials_count   ? function::evaluate
                                                                           : polynomials::getEvaluator(i);
        functions::ExtrCalculator derivative_abs_max = i == polynomials::polynomials_count + 1 ? exponent::derivative_abs_max
                                                       : i == polynomials::polynomials_count
                                                           ? function::derivative_abs_max
                                                           : polynomials::getDerivativeAbsMaxCalculator(i);
        functions::ExtrCalculator second_derivative_abs_max =
            i == polynomials::polynomials_count + 1 ? exponent::second_derivative_abs_max
            : i == polynomials::polynomials_count   ? function::second_derivative_abs_max
                                                    : polynomials::getSecondDerivativeAbsMaxCalculator(i);
        functions::ExtrCalculator fourth_derivative_abs_max =
            i == polynomials::polynomials_count + 1 ? exponent::fourth_derivative_abs_max
            : i == polynomials::polynomials_count   ? function::fourth_derivative_abs_max
                                                    : polynomials::getFourthDerivativeAbsMaxCalculator(i);

        realValue = integrate(a, b);
        long double currentLeftRect = 0;
        long double currentMiddleRect = 0;
        long double currentRightRect = 0;
        long double currentTrapezoid = 0;
        long double currentSimpson = 0;
        long double currentThreeEights = 0;
        if (l == 1) {
            leftRect = currentLeftRect = functions::calculateIntegralUsing::compound::leftRect(evaluate, a, b, m);
            middleRect = currentMiddleRect = functions::calculateIntegralUsing::compound::middleRect(evaluate, a, b, m);
            rightRect = currentRightRect = functions::calculateIntegralUsing::compound::rightRect(evaluate, a, b, m);
            trapezoid = currentTrapezoid = functions::calculateIntegralUsing::compound::trapezoid(evaluate, a, b, m);
            simpson = currentSimpson = functions::calculateIntegralUsing::compound::simpson(evaluate, a, b, m);
            threeEights = currentThreeEights = functions::calculateIntegralUsing::compound::threeEights(evaluate, a, b, m);
        } else {
            currentLeftRect = functions::calculateIntegralUsing::compound::leftRect(evaluate, a, b, m * l);
            currentMiddleRect = functions::calculateIntegralUsing::compound::middleRect(evaluate, a, b, m * l);
            currentRightRect = functions::calculateIntegralUsing::compound::rightRect(evaluate, a, b, m * l);
            currentTrapezoid = functions::calculateIntegralUsing::compound::trapezoid(evaluate, a, b, m * l);
            currentSimpson = functions::calculateIntegralUsing::compound::simpson(evaluate, a, b, m * l);
            currentThreeEights = functions::calculateIntegralUsing::compound::threeEights(evaluate, a, b, m * l);
        }

        std::cout << "> Точное значение: " << realValue << std::endl
                  << std::endl
                  << "> КФ левого прямоугольника:" << std::endl
                  << "    Значение:                         " << currentLeftRect << std::endl
                  << "    Погрешность:                      " << std::abs(realValue - currentLeftRect) << std::endl;
        if (l != 1) {
            long double leftRectPrecise = applyRunge(l, leftRect, currentLeftRect, 1);
            std::cout << "    Уточнённое значение:              " << leftRectPrecise << std::endl
                      << "    Погрешность уточнённого значения: " << std::abs(realValue - leftRectPrecise) << std::endl;
        }
        std::cout << "    Теоретическая погрешность:        "
                  << functions::getTheoreticalErrorOf::compound::leftRect(derivative_abs_max, a, b, m) << std::endl
                  << "> КФ среднего прямоугольника:" << std::endl
                  << "    Значение:                         " << currentMiddleRect << std::endl
                  << "    Погрешность:                      " << std::abs(realValue - currentMiddleRect) << std::endl;
        if (l != 1) {
            long double middleRectPrecise = applyRunge(l, middleRect, currentMiddleRect, 2);
            std::cout << "    Уточнённое значение:              " << middleRectPrecise << std::endl
                      << "    Погрешность уточнённого значения: " << std::abs(realValue - middleRectPrecise) << std::endl;
        }
        std::cout << "    Теоретическая погрешность:        "
                  << functions::getTheoreticalErrorOf::compound::middleRect(second_derivative_abs_max, a, b, m) << std::endl
                  << "> КФ правого прямоугольника:" << std::endl
                  << "    Значение:                         " << currentRightRect << std::endl
                  << "    Погрешность:                      " << std::abs(realValue - currentRightRect) << std::endl;
        if (l != 1) {
            long double rightRectPrecise = applyRunge(l, rightRect, currentRightRect, 1);
            std::cout << "    Уточнённое значение:              " << rightRectPrecise << std::endl
                      << "    Погрешность уточнённого значения: " << std::abs(realValue - rightRectPrecise) << std::endl;
        }
        std::cout << "    Теоретическая погрешность:        "
                  << functions::getTheoreticalErrorOf::compound::rightRect(derivative_abs_max, a, b, m) << std::endl
                  << "> КФ трапеции:" << std::endl
                  << "    Значение:                         " << currentTrapezoid << std::endl
                  << "    Погрешность:                      " << std::abs(realValue - currentTrapezoid) << std::endl;
        if (l != 1) {
            long double trapezoidPrecise = applyRunge(l, trapezoid, currentTrapezoid, 2);
            std::cout << "    Уточнённое значение:              " << trapezoidPrecise << std::endl
                      << "    Погрешность уточнённого значения: " << std::abs(realValue - trapezoidPrecise) << std::endl;
        }
        std::cout << "    Теоретическая погрешность:        "
                  << functions::getTheoreticalErrorOf::compound::trapezoid(second_derivative_abs_max, a, b, m) << std::endl
                  << "> КФ Симпсона:" << std::endl
                  << "    Значение:                         " << currentSimpson << std::endl
                  << "    Погрешность:                      " << std::abs(realValue - currentSimpson) << std::endl;
        if (l != 1) {
            long double simpsonPrecise = applyRunge(l, simpson, currentSimpson, 4);
            std::cout << "    Уточнённое значение:              " << simpsonPrecise << std::endl
                      << "    Погрешность уточнённого значения: " << std::abs(realValue - simpsonPrecise) << std::endl;
        }
        std::cout << "    Теоретическая погрешность:        "
                  << functions::getTheoreticalErrorOf::compound::simpson(fourth_derivative_abs_max, a, b, m) << std::endl
                  << "> КФ трёх восьмых:" << std::endl
                  << "    Значение:                         " << currentThreeEights << std::endl
                  << "    Погрешность:                      " << std::abs(realValue - currentThreeEights) << std::endl;
        if (l != 1) {
            long double threeEightsPrecise = applyRunge(l, threeEights, currentThreeEights, 4);
            std::cout << "    Уточнённое значение:              " << threeEightsPrecise << std::endl
                      << "    Погрешность уточнённого значения: " << std::abs(realValue - threeEightsPrecise) << std::endl;
        }
        std::cout << "    Теоретическая погрешность:        "
                  << functions::getTheoreticalErrorOf::compound::threeEights(fourth_derivative_abs_max, a, b, m) << std::endl;

        std::cout
            << std::endl
            << "Введите l>1 (во сколько раз увеличить число промежутков), 1, чтобы выбрать другую функцию, или 0, чтобы выйти: "
            << std::endl;
        std::cin >> l;
        if (l <= 0) {
            break;
        }
    } while (true);
}
