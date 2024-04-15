#pragma once

class Agent; // 前向声明

class RLAlgorithm {
public:
    virtual ~RLAlgorithm() = default;
    virtual void run(Agent& agent) = 0; // 现在run方法可以访问Agent的数据
};

