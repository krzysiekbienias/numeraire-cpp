#include<vector>
#include"arrays/min_reward.hpp"
#include<numeric>
#include "utils/print_utils_templates.hpp"


int minReward(const std::vector<int>& scores) {
    std::vector<int> rewards(scores.size(), 1);

    // Left to right
    for (int i = 1; i < scores.size(); ++i) {
        if (scores[i] > scores[i - 1]) {
            rewards[i] = rewards[i - 1] + 1;
        }
    }

    // Right to left
    for (int j = scores.size() - 2; j >= 0; --j) {
        if (scores[j] > scores[j + 1]) {
            rewards[j] = std::max(rewards[j], rewards[j + 1] + 1);
        }
    }

    print_utils::printVector(rewards, "Final Rewards");
    return std::accumulate(rewards.begin(), rewards.end(), 0);
}

