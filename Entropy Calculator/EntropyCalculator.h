#ifndef ENTROPY_CALCULATOR_H
#define ENTROPY_CALCULATOR_H

#include <vector>
#include "Probability.h"

class EntropyCalculator {
private:
    std::vector<Probability> probabilities;

    void inputProbabilities();
    bool checkProbabilitySum();
    void clearInputStream();

public:
    double calculateEntropy();
    void displayResults();
    void run();
};

#endif // ENTROPY_CALCULATOR_H