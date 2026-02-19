#include "EntropyCalculator.h"
#include <iostream>
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

                    std::cout << "  Добавлено: " << input << " = "
                        << std::fixed << std::setprecision(4) << value << "\n";
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
            std::cout << "Ошибка: неверный формат вероятности. Примеры: 0.5, 1/2, 0,3\n";
        }

        if (!added) {
            clearInputStream();
        }
    }
}

void EntropyCalculator::inputConditionalProbabilities() {
    std::cout << "\n==================================================\n";
    std::cout << "Введите условные вероятности для каждого a_i:\n";
    std::cout << "Для каждого условия вводите вероятности P(b_j | a_i)\n";
    std::cout << "Формат: b_j=вероятность (например, b1=0.5 или b2=1/2)\n";
    std::cout << "Сумма для каждого условия должна быть =1\n";
    std::cout << "Завершите ввод для условия пустой строкой\n";
    std::cout << "--------------------------------------------------\n";

    for (size_t i = 0; i < probabilities.size(); ++i) {
        std::string condition = "a" + std::to_string(i + 1);
        ConditionalEntropyResult result;
        result.condition = condition;

        std::cout << "\nУсловные вероятности для " << condition << " (P(η|" << condition << " )):\n";

        std::string input;
        int count = 1;
        double current_sum = 0;

        while (true) {
            std::cout << "P(b" << count << "|" << condition << ") (текущая сумма: "
                << std::fixed << std::setprecision(4) << current_sum << "): ";
            std::getline(std::cin, input);

            input.erase(0, input.find_first_not_of(" \t"));
            input.erase(input.find_last_not_of(" \t") + 1);

            if (input.empty()) {
                if (result.probabilities.empty()) {
                    std::cout << "Ошибка: нужно ввести хотя бы одну вероятность!\n";
                    continue;
                }
                if (std::abs(current_sum - 1.0) > 1e-10) {
                    std::cout << "Ошибка: сумма должна быть =1. Текущая сумма: "
                        << std::fixed << std::setprecision(4) << current_sum << "\n";
                    continue;
                }
                break;
            }

            // Парсинг формата b_j=вероятность
            size_t eq_pos = input.find('=');
            if (eq_pos == std::string::npos) {
                std::cout << "Ошибка: формат должен быть b_j=вероятность\n";
                continue;
            }

            std::string event = input.substr(0, eq_pos);
            std::string prob_str = input.substr(eq_pos + 1);

            int num, denom;
            double value;

            if (Probability::parseFraction(prob_str, num, denom)) {
                value = static_cast<double>(num) / denom;
            }
            else if (Probability::parseDecimal(prob_str, value)) {
                // ok
            }
            else {
                std::cout << "Ошибка: неверный формат вероятности.\n";
                continue;
            }

            if (!Probability::isValidProbability(value)) {
                std::cout << "Ошибка: вероятность должна быть в (0,1]\n";
                continue;
            }

            if (current_sum + value > 1.0 + 1e-10) {
                std::cout << "Ошибка: сумма превысит 1. Осталось: "
                    << (1.0 - current_sum) << "\n";
                continue;
            }

            ConditionalProbability cond_p;
            cond_p.event_name = event + "|" + condition;
            cond_p.probability = value;
            cond_p.original_input = prob_str;

            result.probabilities.push_back(cond_p);
            current_sum += value;
            count++;

            std::cout << "  Добавлено: P(" << cond_p.event_name << ") = "
                << std::fixed << std::setprecision(4) << value << "\n";
        }

        result.entropy = Entropy::calculateConditionalEntropy(result.probabilities);
        conditional_results.push_back(result);
    }
}

bool EntropyCalculator::checkProbabilitySum() {
    double sum = 0;
    for (const auto& p : probabilities) {
        sum += p.getDecimal();
    }
    if (std::abs(sum - 1.0) > 1e-10) {
        std::cout << "Ошибка: сумма вероятностей не равна 1 ("
            << std::fixed << std::setprecision(6) << sum << ").\n";
        return false;
    }
    return true;
}

void EntropyCalculator::displayResults() {
    double entropy = Entropy::calculateEntropy(probabilities);
    double total = 0;

    std::cout << "\n==================================================\n";
    std::cout << "РЕЗУЛЬТАТЫ (Обычная энтропия):\n";
    std::cout << "==================================================\n";

    std::cout << "\nВероятности P(ξ = a_i):\n";
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
    double entropy = Entropy::calculateEntropy(probabilities);
    double total = 0;
    double avg_conditional_entropy = Entropy::calculateAverageConditionalEntropy(probabilities, conditional_results);

    std::cout << "\n==================================================\n";
    std::cout << "РЕЗУЛЬТАТЫ (Условная энтропия):\n";
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

    std::cout << "\n3. Средняя (полная) условная энтропия H(η|ξ):\n";
    std::cout << "--------------------------------------------------\n";
    std::cout << "  H(η|ξ) = ";

    // Выводим формулу
    for (size_t i = 0; i < probabilities.size(); i++) {
        if (i > 0) std::cout << " + ";
        std::cout << "p(a" << (i + 1) << ")·H(η|a" << (i + 1) << ")";
    }
    std::cout << "\n\n  ";

    // Выводим вычисление
    for (size_t i = 0; i < probabilities.size(); i++) {
        if (i > 0) std::cout << " + ";
        std::cout << std::fixed << std::setprecision(4)
            << probabilities[i].getDecimal() << "·"
            << std::setprecision(4) << conditional_results[i].entropy;
    }
    std::cout << " = " << std::fixed << std::setprecision(6)
        << avg_conditional_entropy << " бит\n";

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