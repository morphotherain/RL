#pragma once

class Agent; // ǰ������

class RLAlgorithm {
public:
    virtual ~RLAlgorithm() = default;
    virtual void run(Agent& agent) = 0; // ����run�������Է���Agent������
};

