#pragma once
#include "RLAlgorithm.h"
#include "Agent.h"
#include <vector>

class QLearning : public RLAlgorithm
{

private:
	float gamma = 0.99f;
	float alpha = 0.5f;
	float epsilon = 1.0;// 初始ε值
	float epsilonDecay = 0.95; // ε每个episode减小的比例
	float minEpsilon = 0.01; // ε的最小值，以避免ε减小到0;
	int EpisodeLen = 1000;
	int EpisodeNum = 1;
	std::vector<std::vector<std::vector<float>>> qTable; // Q-table

public:

	void run(Agent& agent) override;
	void evaluatePolicy(Agent& agent);
	ActionType chooseActionStochasticWithEpsilon(const std::pair<int, int>& state);
	void decayEpsilon() {
		epsilon = std::max(minEpsilon, epsilon * epsilonDecay);
	}
};
