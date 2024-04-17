#pragma once
#include "RLAlgorithm.h"
#include <vector>

class nStepSarsa : public RLAlgorithm
{

private:
	float gamma = 0.95f;
	float alpha = 0.1f;
	int EpisodeLen = 100;
	int EpisodeNum = 10;
	int stepNum = 1;
	std::vector<std::vector<std::vector<float>>> qTable; // Q-table

public:

	void run(Agent& agent) override;
	void improvePolicyGreedy(Agent& agent);
	void evaluatePolicy(Agent& agent);
};

