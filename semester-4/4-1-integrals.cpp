#include "../lib/functions.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

namespace polynomials
{
    math::polynomials::Polynomial polynomials[] = {
        std::vector {5.0l},
        std::vector {8.0l, 9.0l},
        std::vector {2.0l, -24.0l, 3.0l},
        std::vector {-15.0l, 0.0l, -30.0l, 3.0l}};
    constexpr size_t polynomials_count = std::size(polynomials);

    math::integrator_t getIntegrator(size_t i)
    {
        return [&, i](long double a, long double b) {
            return polynomials[i].integrate(a, b);
        };
    }

    math::simple_function_t getEvaluator(size_t i)
    {
        return polynomials[i];
    }
} // namespace polynomials

namespace function
{
    std::string str = "5/(1 + e^(-x))";

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
} // namespace function

int main()
{
    std::cout << std::setprecision(20);
    do {
        size_t i;

        std::cout << "> Выберите функцию:" << std::endl;
        for (size_t i = 0; i < polynomials::polynomials_count; i++) {
            std::cout << (i + 1) << ". " << static_cast<std::string>(polynomials::polynomials[i]) << std::endl;
        }
        std::cout << (polynomials::polynomials_count + 1) << ". " << function::str << std::endl;
        do {
            std::cin >> i;
            i--;

            if (i >= 0 && i <= polynomials::polynomials_count) {
                break;
            } else {
                std::cout << "! Недопустимый номер функции, введите ещё раз:" << std::endl;
            }
        } while (true);

        long double a, b;
        std::cout << "> Введите пределы интегрирования (положительные числа через "
                     "пробел или перевод строки)"
                  << std::endl;
        std::cin >> a >> b;

        math::integrator_t integrate = i == polynomials::polynomials_count ? function::integrate : polynomials::getIntegrator(i);
        math::simple_function_t evaluate =
            i == polynomials::polynomials_count ? function::evaluate : polynomials::getEvaluator(i);

        long double realValue = integrate(a, b);
        long double leftRect = math::calculateIntegralUsing::leftRect(evaluate, a, b);
        long double middleRect = math::calculateIntegralUsing::middleRect(evaluate, a, b);
        long double rightRect = math::calculateIntegralUsing::rightRect(evaluate, a, b);
        long double trapezoid = math::calculateIntegralUsing::trapezoid(evaluate, a, b);
        long double simpson = math::calculateIntegralUsing::simpson(evaluate, a, b);
        long double threeEights = math::calculateIntegralUsing::threeEights(evaluate, a, b);

        std::cout << std::endl
                  << "> Точное значение: " << realValue << std::endl
                  << std::endl
                  << "> КФ левого прямоугольника:" << std::endl
                  << "    Значение: " << leftRect << std::endl
                  << "    Погрешность: " << std::abs(realValue - leftRect) << std::endl
                  << "> КФ среднего прямоугольника:" << std::endl
                  << "    Значение: " << middleRect << std::endl
                  << "    Погрешность: " << std::abs(realValue - middleRect) << std::endl
                  << "> КФ правого прямоугольника:" << std::endl
                  << "    Значение: " << rightRect << std::endl
                  << "    Погрешность: " << std::abs(realValue - rightRect) << std::endl
                  << "> КФ трапеции:" << std::endl
                  << "    Значение: " << trapezoid << std::endl
                  << "    Погрешность: " << std::abs(realValue - trapezoid) << std::endl
                  << "> КФ Симпсона:" << std::endl
                  << "    Значение: " << simpson << std::endl
                  << "    Погрешность: " << std::abs(realValue - simpson) << std::endl
                  << "> КФ трёх восьмых:" << std::endl
                  << "    Значение: " << threeEights << std::endl
                  << "    Погрешность: " << std::abs(realValue - threeEights) << std::endl;

        char c;
        std::cout << std::endl << "> Запустить программу ещё раз? [y/n]: ";
        std::cin >> c;
        std::cout << std::endl;

        if (c != 'Y' && c != 'y') {
            break;
        }
    } while (true);
}
