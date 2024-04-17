#pragma once
#include "RLAlgorithm.h"
#include <vector>
class Sarsa : public RLAlgorithm
{

private:
	float gamma = 0.95f;
	float alpha = 0.1f;
	int EpisodeLen = 100;
	int EpisodeNum = 10;
	std::vector<std::vector<std::vector<float>>> qTable; // Q-table

public:
	Sarsa() {
		qTable.resize(10, std::vector<std::vector<float>>(10, std::vector<float>(5, 0.0f))); // 10x10网格，5个动作
	}
	void run(Agent& agent) override;
	void improvePolicyGreedy(Agent& agent);
	void evaluatePolicy(Agent& agent);
};

