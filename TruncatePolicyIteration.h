#pragma once
#include "RLAlgorithm.h"
class TruncatePolicyIteration : public RLAlgorithm
{
public:
	void run(Agent& agent) override;
};


