#pragma once

#include "RLAlgorithm.h"
class MC��Greedy : public RLAlgorithm
{
private:
	float gamma = 0.9f;
	float �� = 0.1;
public:
	void run(Agent& agent) override;
};

