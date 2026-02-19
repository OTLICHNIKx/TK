#include "Entropy.h"
#include <cmath>

namespace Entropy {

    double calculateEntropy(const std::vector<Probability>& probabilities) {
        double entropy = 0;
        for (const auto& p : probabilities) {
            double p_val = p.getDecimal();
            if (p_val > 0) {
                entropy -= p_val * std::log2(p_val);
            }
        }
        return entropy;
    }

    double calculateConditionalEntropy(const std::vector<ConditionalProbability>& cond_probs) {
        double entropy = 0;
        for (const auto& p : cond_probs) {
            if (p.probability > 0) {
                entropy -= p.probability * std::log2(p.probability);
            }
        }
        return entropy;
    }

    double calculateAverageConditionalEntropy(
        const std::vector<Probability>& probabilities,
        const std::vector<ConditionalEntropyResult>& conditional_results) {
        double avg_entropy = 0;
        for (size_t i = 0; i < probabilities.size(); i++) {
            double p_i = probabilities[i].getDecimal();
            double h_eta_a_i = conditional_results[i].entropy;
            avg_entropy += p_i * h_eta_a_i;
        }
        return avg_entropy;
    }

    double calculateJointEntropy(const std::vector<JointProbability>& joint_probs) {
        double entropy = 0.0;
        for (const auto& jp : joint_probs) {
            double p = jp.probability;
            if (p > 0.0) {
                entropy -= p * std::log2(p);
            }
        }
        return entropy;
    }

} // namespace Entropy