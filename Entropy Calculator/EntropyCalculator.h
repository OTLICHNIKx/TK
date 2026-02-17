#ifndef ENTROPY_CALCULATOR_H
#define ENTROPY_CALCULATOR_H

#include <vector>
#include <string>
#include "Probability.h"

// Условная вероятность
struct ConditionalProbability {
    std::string event_name;           // название события (a_i | b_j)
    double probability;               // условная вероятность
    std::string original_input;       // исходное представление
};

// Результат условной вероятности
struct ConditionalEntropyResult {
    std::string condition;                             // условие (a_i)
    double entropy;                                    // частная условная энтропия H(η|a_i)
    std::vector<ConditionalProbability> probabilities; // условные вероятности
};

class EntropyCalculator {
private:
    std::vector<Probability> probabilities;                    // безусловные вероятности p(a_i)
    std::vector<ConditionalEntropyResult> conditional_results; // результаты для каждого условия

    void inputProbabilities();
    void inputConditionalProbabilities();
    bool checkProbabilitySum();
    void clearInputStream();

    double calculateConditionalEntropy(const std::vector<ConditionalProbability>& cond_probs);
    double calculateAverageConditionalEntropy();
    int showMenu();

public:
    double calculateEntropy();
    void calculateAllConditionalEntropies();
    void displayResults();
    void displayConditionalResults();
    void run();
};

#endif // ENTROPY_CALCULATOR_H