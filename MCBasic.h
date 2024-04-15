#pragma once
#include "RLAlgorithm.h"
class MCBasic : public RLAlgorithm
{
private:
	float gamma = 0.9f;
public:
	void run(Agent& agent) override;
};

