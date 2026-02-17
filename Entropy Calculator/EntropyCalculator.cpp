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

int EntropyCalculator::showMenu() {
    while (true) {
        std::cout << "==================================================\n";
        std::cout << "            КАЛЬКУЛЯТОР ЭНТРОПИИ\n";
        std::cout << "==================================================\n";
        std::cout << "Выберите режим работы:\n";
        std::cout << "1. Обычная энтропия H(ξ)\n";
        std::cout << "2. Частная условная энтропия H(η|a_i)\n";
        std::cout << "0. Выход\n";
        std::cout << "--------------------------------------------------\n";
        std::cout << "Ваш выбор: ";

        std::string input;
        std::getline(std::cin, input);

        // Удаляем пробелы
        input.erase(0, input.find_first_not_of(" \t"));
        input.erase(input.find_last_not_of(" \t") + 1);

        // Проверяем, что ввод не пустой и состоит только из цифр
        bool is_number = !input.empty() &&
            std::all_of(input.begin(), input.end(), ::isdigit);

        if (!is_number) {
            std::cout << "Ошибка: введите число (0, 1 или 2).\n\n";
            continue;
        }

        int choice = std::stoi(input);

        if (choice >= 0 && choice <= 2) {
            return choice;
        }

        std::cout << "Неверный выбор. Пожалуйста, выберите 1, 2 или 0.\n\n";
    }
}

void EntropyCalculator::inputProbabilities() {
    std::string input;
    int count = 1;
    double current_sum = 0;

    std::cout << "==================================================\n";
    std::cout << "Введите вероятности:\n";
    std::cout << "(поддерживаются форматы: 0,3, 1/2, 0.25, и т.д.)\n";
    std::cout << "Сумма вероятностей должна быть равна 1\n";
    std::cout << "Для завершения ввода введите пустую строку\n";
    std::cout << "--------------------------------------------------\n";

    while (true) {
        std::cout << "p" << count << " (текущая сумма: "
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

                    std::cout << "  Добавлено: " << num << "/" << denom << " = "
                        << std::fixed << std::setprecision(4) << p << "\n";
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

                    std::cout << "  Добавлено: " << std::fixed << std::setprecision(4) << value << "\n";
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

void EntropyCalculator::inputConditionalProbabilities() {
    std::cout << "\n==================================================\n";
    std::cout << "Введите условные вероятности P(η = b_j | ξ = a_i):\n";
    std::cout << "--------------------------------------------------\n";

    for (size_t i = 0; i < probabilities.size(); i++) {
        ConditionalEntropyResult result;
        result.condition = "a" + std::to_string(i + 1);

        std::cout << "\nДля условия ξ = a" << (i + 1) << ":\n";

        int count = 1;
        double current_sum = 0;

        while (true) {
            std::cout << "  P(b" << count << " | a" << (i + 1) << ") (текущая сумма: "
                << std::fixed << std::setprecision(4) << current_sum << "): ";

            std::string input;
            std::getline(std::cin, input);

            // Удаляем пробелы в начале и конце
            input.erase(0, input.find_first_not_of(" \t"));
            input.erase(input.find_last_not_of(" \t") + 1);

            if (input.empty()) {
                if (count == 1) {
                    std::cout << "    Ошибка: нужно ввести хотя бы одну условную вероятность!\n";
                    continue;
                }
                if (std::abs(current_sum - 1.0) > 1e-10) {
                    std::cout << "    Ошибка: сумма условных вероятностей должна быть равна 1. Текущая сумма: "
                        << std::fixed << std::setprecision(4) << current_sum << "\n";
                    std::cout << "    Введите оставшуюся вероятность или начните заново для этого условия.\n";
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
                        ConditionalProbability cond_prob;
                        cond_prob.event_name = "b" + std::to_string(count) + "|a" + std::to_string(i + 1);
                        cond_prob.probability = p;
                        cond_prob.original_input = input;
                        result.probabilities.push_back(cond_prob);

                        current_sum += p;
                        count++;
                        added = true;

                        std::cout << "    Добавлено: " << p << "\n";
                    }
                    else {
                        std::cout << "    Ошибка: сумма вероятностей не может превышать 1. Осталось: "
                            << std::fixed << std::setprecision(4) << (1.0 - current_sum) << "\n";
                    }
                }
                else {
                    std::cout << "    Ошибка: вероятность должна быть в интервале (0, 1]\n";
                }
            }
            // Пробуем как десятичное число
            else if (Probability::parseDecimal(input, value)) {
                if (Probability::isValidProbability(value)) {
                    if (current_sum + value <= 1.0 + 1e-10) {
                        ConditionalProbability cond_prob;
                        cond_prob.event_name = "b" + std::to_string(count) + "|a" + std::to_string(i + 1);
                        cond_prob.probability = value;
                        cond_prob.original_input = input;
                        result.probabilities.push_back(cond_prob);

                        current_sum += value;
                        count++;
                        added = true;

                        std::cout << "    Добавлено: " << value << "\n";
                    }
                    else {
                        std::cout << "    Ошибка: сумма вероятностей не может превышать 1. Осталось: "
                            << std::fixed << std::setprecision(4) << (1.0 - current_sum) << "\n";
                    }
                }
                else {
                    std::cout << "    Ошибка: вероятность должна быть в интервале (0, 1]\n";
                }
            }
            else {
                std::cout << "    Ошибка: неверный формат. Используйте например: 0,3, 1/2, 0,75\n";
            }

            if (added && std::abs(current_sum - 1.0) < 1e-10) {
                std::cout << "    Сумма условных вероятностей = 1\n";
                break;
            }
        }

        // Вычисляем частную условную энтропию для этого условия
        result.entropy = calculateConditionalEntropy(result.probabilities);
        conditional_results.push_back(result);
    }
}

double EntropyCalculator::calculateConditionalEntropy(const std::vector<ConditionalProbability>& cond_probs) {
    double entropy = 0;
    for (const auto& p : cond_probs) {
        if (p.probability > 0) {
            entropy -= p.probability * log2(p.probability);
        }
    }
    return entropy;
}

void EntropyCalculator::calculateAllConditionalEntropies() {
    for (auto& result : conditional_results) {
        result.entropy = calculateConditionalEntropy(result.probabilities);
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
    std::cout << "РЕЗУЛЬТАТЫ (Обычная энтропия):\n";
    std::cout << "==================================================\n";

    std::cout << "\nВероятности:\n";
    std::cout << "--------------------------------------------------\n";
    for (size_t i = 0; i < probabilities.size(); i++) {
        std::cout << "  p" << (i + 1) << " = ";
        if (probabilities[i].isFraction()) {
            std::cout << probabilities[i].getOriginal() << " ≈ ";
        }
        std::cout << std::fixed << std::setprecision(4)
            << probabilities[i].getDecimal() << "\n";
        total += probabilities[i].getDecimal();
    }
    std::cout << "\n  Сумма: " << std::fixed << std::setprecision(6) << total << "\n";
    std::cout << "  Энтропия H = " << std::fixed << std::setprecision(6)
        << entropy << " бит\n";

    std::cout << "==================================================\n";
}

void EntropyCalculator::displayConditionalResults() {
    double entropy = calculateEntropy();
    double total = 0;

    std::cout << "\n==================================================\n";
    std::cout << "РЕЗУЛЬТАТЫ (Частная условная энтропия):\n";
    std::cout << "==================================================\n";

    std::cout << "\n1. Безусловные вероятности P(ξ = a_i):\n";
    std::cout << "--------------------------------------------------\n";
    for (size_t i = 0; i < probabilities.size(); i++) {
        std::cout << "  P(a" << (i + 1) << ") = ";
        if (probabilities[i].isFraction()) {
            std::cout << probabilities[i].getOriginal() << " ≈ ";
        }
        std::cout << std::fixed << std::setprecision(4)
            << probabilities[i].getDecimal() << "\n";
        total += probabilities[i].getDecimal();
    }
    std::cout << "\n  Сумма: " << std::fixed << std::setprecision(6) << total << "\n";
    std::cout << "  Энтропия H(ξ) = " << std::fixed << std::setprecision(6)
        << entropy << " бит\n";

    std::cout << "\n2. Частные условные энтропии H(η|a_i):\n";
    std::cout << "--------------------------------------------------\n";
    for (const auto& result : conditional_results) {
        std::cout << "\n  Для условия ξ = " << result.condition << ":\n";

        double cond_total = 0;
        for (const auto& p : result.probabilities) {
            std::cout << "    P(" << p.event_name << ") = "
                << std::fixed << std::setprecision(4) << p.probability << "\n";
            cond_total += p.probability;
        }
        std::cout << "    Сумма: " << std::fixed << std::setprecision(6) << cond_total << "\n";
        std::cout << "    H(η|" << result.condition << ") = "
            << std::fixed << std::setprecision(6) << result.entropy << " бит\n";
    }

    std::cout << "\n==================================================\n";
}

void EntropyCalculator::run() {
    int choice;

    while (true) {
        choice = showMenu();

        if (choice == 0) {
            std::cout << "Программа завершена.\n";
            return;
        }

        if (choice == 1 || choice == 2) {
            break; // корректный выбор, выходим из цикла
        }

        std::cout << "Неверный выбор. Пожалуйста, выберите 1, 2 или 0.\n\n";
    }

    // Очищаем векторы перед новым расчетом
    probabilities.clear();
    conditional_results.clear();

    // Ввод безусловных вероятностей (общий для обоих режимов)
    inputProbabilities();

    if (!checkProbabilitySum()) {
        std::cout << "Вычисления отменены\n";
        return;
    }

    if (choice == 1) {
        // Обычная энтропия
        displayResults();
    }
    else if (choice == 2) {
        // Частная условная энтропия
        inputConditionalProbabilities();
        displayConditionalResults();
    }

    std::cout << "\n";
}