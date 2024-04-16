#pragma once
#include "RLAlgorithm.h"
class StochasticGradientDescent : public RLAlgorithm
{
private:
	float gamma = 0.9f;
	float alpha = 0.05f; // 学习率
	int EpisodeLen = 20;
	int EpisodeNum = 1;//和batch gradient descent相比, 就是把n设为1;
public:
	void run(Agent& agent) override;
	void improvePolicyGreedy(Agent& agent);
	void evaluatePolicy(Agent& agent);

};

