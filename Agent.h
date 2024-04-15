#pragma once
#include <map>
#include <vector>
#include "Environment.h"
#include "RLAlgorithm.h"
#include "BellmanSolve.h"
#include "BellmanOptimalitySolve.h"
#include "TruncatePolicyIteration.h"
#include "MCBasic.h"
#include <cstdlib> // 包含rand()和srand()
#include <ctime> // 包含time()


class Agent {
private:
    Environment* environment;

    RLAlgorithm* algorithm = nullptr;

    // 随机性策略：对于每个状态，存储每个动作的概率
    std::map<std::pair<int, int>, std::map<ActionType, float>> stochasticPolicy;

    std::map<ActionType, std::pair<int, int>> actionSpace; // 动作空间

    int stepCount = 0; // 步计数


public:
    Agent(Environment* env);
    ~Agent();

    void setAlgorithm(RLAlgorithm* newAlgorithm);

    void run();

    void initializeDeterministicPolicy(int gridSize);

    void initializeStochasticPolicy(int gridSize);

    void initializeActionSpace(); // 初始化动作空间

    std::map<ActionType, std::pair<int, int>> getActionSpace();


    // 设置随机性策略的方法
    void setStochasticPolicy(const std::pair<int, int>& state, const std::map<ActionType, float>& actionProbabilities);

    // 根据随机性策略选择动作（伪代码）
    ActionType chooseActionStochastic(const std::pair<int, int>& state);
    
    // 获取某状态的随机性策略
    std::map<ActionType, float> getStochasticPolicy(const std::pair<int, int>& state) const;


    int getStepCount() const;

    Environment* getEnvironment() { return environment; }

    std::pair<int, int> getNextState(const std::pair<int, int>& currentState, ActionType action);
};
