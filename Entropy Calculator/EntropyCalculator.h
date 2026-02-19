#ifndef ENTROPY_CALCULATOR_H
#define ENTROPY_CALCULATOR_H

#include <vector>
#include <string>
#include "Probability.h"
#include "Entropy.h"

class EntropyCalculator {
private:
    std::vector<Probability> probabilities;                    // безусловные вероятности p(a_i)
    std::vector<ConditionalEntropyResult> conditional_results; // результаты для каждого условия

    void inputProbabilities();
    void inputConditionalProbabilities();
    bool checkProbabilitySum();
    void clearInputStream();
    int showMenu();
public:
    void displayResults();
    void displayConditionalResults();
    void run();
};

#endif // ENTROPY_CALCULATOR_H