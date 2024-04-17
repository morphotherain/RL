#pragma once
#include "RLAlgorithm.h"
#include "Agent.h"
#include <vector>

class QLearning : public RLAlgorithm
{

private:
	float gamma = 0.99f;
	float alpha = 0.5f;
	float epsilon = 1.0;// ��ʼ��ֵ
	float epsilonDecay = 0.95; // ��ÿ��episode��С�ı���
	float minEpsilon = 0.01; // �ŵ���Сֵ���Ա���ż�С��0;
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
