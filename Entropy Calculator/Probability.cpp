#include "Probability.h"
#include <algorithm>
#include <iostream>
#include <limits>

Probability::Probability(double value, const std::string& input)
    : decimal_value(value), original_input(input), is_fraction(false) {
}

Probability::Probability(int numerator, int denominator, const std::string& input)
    : original_input(input), is_fraction(true) {
    decimal_value = static_cast<double>(numerator) / denominator;
}

double Probability::getDecimal() const {
    return decimal_value;
}

std::string Probability::getOriginal() const {
    return original_input;
}

bool Probability::isFraction() const {
    return is_fraction;
}

bool Probability::parseFraction(const std::string& input, int& num, int& denom) {
    size_t slash_pos = input.find('/');
    if (slash_pos == std::string::npos) return false;
    try {
        num = std::stoi(input.substr(0, slash_pos));
        denom = std::stoi(input.substr(slash_pos + 1));
        return denom != 0;
    }
    catch (...) {
        return false;
    }
}

bool Probability::parseDecimal(const std::string& input, double& value) {
    std::string normalized = input;

    // Удаляем пробелы
    normalized.erase(remove(normalized.begin(), normalized.end(), ' '), normalized.end());

    if (normalized.empty()) return false;

    // Пробуем распарсить с текущим разделителем
    try {
        size_t pos;
        value = std::stod(normalized, &pos);
        if (pos == normalized.length()) {
            return true;
        }
    }
    catch (...) {
        // Игнорируем ошибку и пробуем другой вариант
    }

    // Если не получилось, пробуем заменить запятую на точку или наоборот
    try {
        if (normalized.find(',') != std::string::npos) {
            // Если есть запятая, заменяем на точку
            std::replace(normalized.begin(), normalized.end(), ',', '.');
        }
        else if (normalized.find('.') != std::string::npos) {
            // Если есть точка, заменяем на запятую
            std::replace(normalized.begin(), normalized.end(), '.', ',');
        }
        else {
            return false;
        }

        size_t pos;
        value = std::stod(normalized, &pos);
        return pos == normalized.length();
    }
    catch (...) {
        return false;
    }
}

bool Probability::isValidProbability(double p) {
    return p > 0 && p <= 1.0;
}