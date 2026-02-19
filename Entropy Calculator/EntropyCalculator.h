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
    std::vector<JointProbability> joint_probabilities;         // p(a_i, b_j)
    std::vector<double> marginal_probabilities_eta;            // P(η = b_j) — опционально

    void inputProbabilities();
    void inputConditionalProbabilities();
    bool checkProbabilitySum();
    void clearInputStream();
    void computeJointProbabilities();                          

    int showMenu();
public:
    void displayResults();
    void displayConditionalResults();
    void displayJointEntropyResults();                         

    void run();
};

#endif // ENTROPY_CALCULATOR_H