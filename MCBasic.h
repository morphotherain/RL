#pragma once
#include "RLAlgorithm.h"
class MCBasic : public RLAlgorithm
{
private:
	float gamma = 0.9f;
	int EpisodeLen = 20;
	int EpisodeNum = 2;
public:
	void run(Agent& agent) override;
};

