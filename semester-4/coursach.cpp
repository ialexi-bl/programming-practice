#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct Sum;
struct Product;
struct Monomial;
struct Polynomial;
struct AgebraicPolynomial;

struct Product {
    int coefficient = 1;
    std::map<std::string, int> factors;

    Product(int k) : coefficient(k)
    {
    }
    Product(const std::map<std::string, int> &factors) : factors(factors)
    {
    }
    Product(int k, const std::map<std::string, int> &factors) : coefficient(k), factors(factors)
    {
    }
};

struct Sum {
    std::vector<Product> terms;

    Sum(const Product &product) : terms{product}
    {
    }
};

Product operator*(const Product &p1, const Product &p2)
{
    std::map<std::string, int> new_factors = p1.factors;

    for (auto factor = p2.factors.begin(); factor != p2.factors.end(); factor++) {
        auto it = new_factors.find(factor->first);
        if (it != p2.factors.end()) {
            new_factors[factor->first] += factor->second;
        } else {
            new_factors[factor->first] = factor->second;
        }
    }
    return {p1.coefficient * p2.coefficient, new_factors};
}

Sum operator+(const Sum &s1, const Sum &s2)
{
    std::vector<Product> terms;
    for (auto term = s1.terms.begin(); term != s2.terms.end(); term++) {
        auto it = s2.terms.find(term->first);
        if (it != s2.terms.end()) {
            terms.push_back({term->second.coefficient + it->second.coefficient, term->second.factors});
        } else {
            terms.push_back(term->second);
        }
    }
    for (auto term = s2.terms.begin(); term != s2.terms.end(); term++) {
        auto it = terms.find(term->first);
        if (it == terms.end()) {
            terms.push_back(term->second);
        }
    }
}

struct Monomial {
    Sum numerator;
    Sum denominator;

    Monomial(const Sum &numerator) : numerator(numerator)
    {
    }
};

Monomial operator*(const Monomial &m1, const Monomial &m2)
{
    return {
        .numerator = m1.numerator * m2.numerator,
        .denominator = m1.denominator * m2.denominator,
    };
}
Monomial operator/(const Monomial &m1, const Monomial &m2)
{
    return {
        .numerator = m1.numerator * m2.denominator,
        .denominator = m1.denominator * m2.numerator,
    };
}

struct Polynomial {
    std::vector<Monomial> terms;
};
struct AlgebraicPolynomial : Polynomial {
};

Sum getCoefSecondPower(const AlgebraicPolynomial &p, int order)
{
    Sum result;
    int size = p.terms.size();
    for (int i = 0; i < size; i++) {
        int j = order - i;
        result += p.terms[i] * p.terms[j];
    }
    return result;
}
Sum getCoefThirdPower(const AlgebraicPolynomial &p, int order)
{
    Sum result;
    int size = p.terms.size();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j <= order - i; j++) {
            int k = order - i - j;
            result += p.terms[i] * p.terms[j] * p.terms[k];
        }
    }
    return result;
}

int main()
{
    int order = 3;
    AlgebraicPolynomial result{{Monomial{0}, Monomial{.numerator = {-1}, .denominator = {{"c", 1}}}}};

    for (int i = 2; i <= order; i++) {
        Monomial term{.numerator = -1, .denominator = {{"c", 1}}};
        term *= getCoefThirdPower(result, i) + Monomial{{"b", 1}} * getCoefSecondPower(result, i);
        result.terms.push_back(term);
    }

    return 0;
}