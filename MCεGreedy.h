#pragma once

#include "RLAlgorithm.h"
class MC¦ÅGreedy : public RLAlgorithm
{
private:
	float gamma = 0.9f;
	float ¦Å = 0.1;
public:
	void run(Agent& agent) override;
};

