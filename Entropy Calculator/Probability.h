#ifndef PROBABILITY_H
#define PROBABILITY_H

#include <string>

class Probability {
private:
    double decimal_value;       // десятичная запись
    std::string original_input; // исходное представление вероятности
    bool is_fraction;           // является ли дробью

public:
    Probability(double value, const std::string& input);
    Probability(int numerator, int denominator, const std::string& input);

    double getDecimal() const;
    std::string getOriginal() const;
    bool isFraction() const;

    // Статические методы для парсинга и валидации
    static bool parseFraction(const std::string& input, int& num, int& denom);
    static bool parseDecimal(const std::string& input, double& value);
    static bool isValidProbability(double p);
};

#endif // PROBABILITY_H