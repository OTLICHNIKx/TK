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

// Совместная вероятность
struct JointProbability {
    std::string pair_name;      
    double probability;         // p(a_i, b_j)
    std::string original_input; 
};

// Результат совместной энтропии
struct JointEntropyResult {
    double joint_entropy;                          // H(ξ, η)
    std::vector<JointProbability> joint_probs;     // все ненулевые совместные вероятности
    std::vector<double> marginal_eta;              // P(η = b_j) для каждого b_j
};

namespace Entropy {

    // Расчет обычной энтропии H(ξ)
    double calculateEntropy(const std::vector<Probability>& probabilities);

    // Расчет частной условной энтропии H(η|a_i)
    double calculateConditionalEntropy(const std::vector<ConditionalProbability>& cond_probs);

    // Расчет средней полной энтропии H(η|ξ)
    double calculateAverageConditionalEntropy(
        const std::vector<Probability>& probabilities,
        const std::vector<ConditionalEntropyResult>& conditional_results);
    
    // Расчет совместной энтропии H(ξ,η)
    double calculateJointEntropy(const std::vector<JointProbability>& joint_probs);

} // namespace Entropy

#endif // ENTROPY_H