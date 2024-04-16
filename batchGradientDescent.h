#pragma once
#include "RLAlgorithm.h"
class batchGradientDescent :   public RLAlgorithm
{
private:
	float gamma = 0.9f;
	float alpha = 0.05f; // ѧϰ��
	int EpisodeLen = 20;
	int EpisodeNum = 5;
public:
	void run(Agent& agent) override;
	void improvePolicyGreedy(Agent& agent);
	void evaluatePolicy(Agent& agent);
};

