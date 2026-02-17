#include "EntropyCalculator.h"
#include <iostream>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <limits>

void EntropyCalculator::clearInputStream() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void EntropyCalculator::inputProbabilities() {
    std::string input;
    int count = 1;
    double current_sum = 0;

    std::cout << "Калькулятор энтропии\n";
    std::cout << "==================================================\n";
    std::cout << "Введите вероятности (поддерживаются форматы: 0,3, 1/2, 0,25, 3/4 и т.д.)\n";
    std::cout << "Сумма вероятностей должна быть равна 1\n";
    std::cout << "Для завершения ввода введите пустую строку\n";
    std::cout << "--------------------------------------------------\n";

    while (true) {
        std::cout << "Вероятность " << count << " (текущая сумма: "
            << std::fixed << std::setprecision(4) << current_sum << "): ";
        std::getline(std::cin, input);

        // Удаляем пробелы в начале и конце
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        if (input.empty()) {
            if (probabilities.empty()) {
                std::cout << "Ошибка: нужно ввести хотя бы одну вероятность!\n";
                continue;
            }
            if (std::abs(current_sum - 1.0) > 1e-10) {
                std::cout << "Ошибка: сумма вероятностей должна быть равна 1. Текущая сумма: "
                    << std::fixed << std::setprecision(4) << current_sum << "\n";
                std::cout << "Введите оставшуюся вероятность или начните заново.\n";
                continue;
            }
            break;
        }

        int num, denom;
        double value;
        bool added = false;

        // Пробуем распарсить как дробь
        if (Probability::parseFraction(input, num, denom)) {
            double p = static_cast<double>(num) / denom;
            if (Probability::isValidProbability(p)) {
                if (current_sum + p <= 1.0 + 1e-10) {
                    probabilities.push_back(Probability(num, denom, input));
                    current_sum += p;
                    count++;
                    added = true;
                }
                else {
                    std::cout << "Ошибка: сумма вероятностей не может превышать 1. Осталось: "
                        << std::fixed << std::setprecision(4) << (1.0 - current_sum) << "\n";
                }
            }
            else {
                std::cout << "Ошибка: вероятность должна быть в интервале (0, 1]\n";
            }
        }
        // Пробуем как десятичное число
        else if (Probability::parseDecimal(input, value)) {
            if (Probability::isValidProbability(value)) {
                if (current_sum + value <= 1.0 + 1e-10) {
                    probabilities.push_back(Probability(value, input));
                    current_sum += value;
                    count++;
                    added = true;
                }
                else {
                    std::cout << "Ошибка: сумма вероятностей не может превышать 1. Осталось: "
                        << std::fixed << std::setprecision(4) << (1.0 - current_sum) << "\n";
                }
            }
            else {
                std::cout << "Ошибка: вероятность должна быть в интервале (0, 1]\n";
            }
        }
        else {
            std::cout << "Ошибка: неверный формат. Используйте например: 0,3, 1/2, 0,75\n";
        }

        // Проверяем сумму после добавления
        if (added) {
            if (std::abs(current_sum - 1.0) < 1e-10) {
                std::cout << "Сумма вероятностей = 1\n";
                break;
            }
            // Если сумма меньше 1, просто продолжаем цикл
        }
    }
}

bool EntropyCalculator::checkProbabilitySum() {
    double total = 0;
    for (const auto& p : probabilities) {
        total += p.getDecimal();
    }

    if (std::abs(total - 1.0) > 1e-10) {
        std::cout << "\nОшибка: сумма вероятностей = "
            << std::fixed << std::setprecision(6) << total
            << " (должна быть равна 1)\n";
        std::cout << "Программа завершена с ошибкой.\n";
        return false;
    }

    return true;
}

double EntropyCalculator::calculateEntropy() {
    double entropy = 0;
    for (const auto& p : probabilities) {
        double p_val = p.getDecimal();
        if (p_val > 0) {
            entropy -= p_val * log2(p_val);
        }
    }
    return entropy;
}

void EntropyCalculator::displayResults() {
    double entropy = calculateEntropy();
    double total = 0;

    std::cout << "\n==================================================\n";
    std::cout << "РЕЗУЛЬТАТЫ:\n";
    std::cout << "--------------------------------------------------\n";

    std::cout << "Введенные вероятности:\n";
    for (size_t i = 0; i < probabilities.size(); i++) {
        std::cout << "  p" << (i + 1) << " = ";
        if (probabilities[i].isFraction()) {
            std::cout << probabilities[i].getOriginal() << " ≈ ";
        }
        std::cout << std::fixed << std::setprecision(4)
            << probabilities[i].getDecimal() << "\n";
        total += probabilities[i].getDecimal();
    }

    std::cout << "\nСумма вероятностей: " << std::fixed << std::setprecision(6)
        << total << "\n";
    std::cout << "\nЭнтропия H = " << std::fixed << std::setprecision(6)
        << entropy << " бит\n";
    std::cout << "==================================================\n";
}

void EntropyCalculator::run() {
    inputProbabilities();

    if (!checkProbabilitySum()) {
        std::cout << "Вычисления отменены\n";
        return;
    }

    displayResults();
}