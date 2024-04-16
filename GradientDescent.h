#pragma once

#include "RLAlgorithm.h"
class GradientDescent : public RLAlgorithm
{
private:
	float gamma = 0.9f;
	float alpha = 0.05f; // ѧϰ��
public:
	void run(Agent& agent) override;
};

