#ifndef ENTROPY_H
#define ENTROPY_H

#include <vector>
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

namespace Entropy {

    // Расчет обычной энтропии H(ξ)
    double calculateEntropy(const std::vector<Probability>& probabilities);

    // Расчет частной условной энтропии H(η|a_i)
    double calculateConditionalEntropy(const std::vector<ConditionalProbability>& cond_probs);

    // Расчет средней условной энтропии H(η|ξ)
    double calculateAverageConditionalEntropy(
        const std::vector<Probability>& probabilities,
        const std::vector<ConditionalEntropyResult>& conditional_results);

} // namespace Entropy

#endif // ENTROPY_H