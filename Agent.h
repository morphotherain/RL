#pragma once
#include <map>
#include <vector>
#include "Environment.h"
#include "RLAlgorithm.h"
#include "BellmanSolve.h"
#include "BellmanOptimalitySolve.h"
#include "TruncatePolicyIteration.h"
#include "MCBasic.h"
#include "MCExplore.h"
#include "MCεGreedy.h"
#include "GradientDescent.h"
#include "batchGradientDescent.h"
#include "StochasticGradientDescent.h"
#include "Sarsa.h"
#include "nStepSarsa.h"
#include "QLearning.h"
#include <cstdlib> // 包含rand()和srand()
#include <ctime> // 包含time()

struct EpisodeStep {
    std::pair<int, int> state;
    ActionType action;
    float reward;
};


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

    void initializeDeterministicPolicy(Environment* env);

    void initializeStochasticPolicy(Environment* env);

    void initializeActionSpace(); // 初始化动作空间

    std::map<ActionType, std::pair<int, int>> getActionSpace();


    // 设置随机性策略的方法
    void setStochasticPolicy(const std::pair<int, int>& state, const std::map<ActionType, float>& actionProbabilities);

    // 根据随机性策略选择动作
    ActionType chooseActionStochastic(const std::pair<int, int>& state);


    
    // 获取某状态的随机性策略
    std::map<ActionType, float> getStochasticPolicy(const std::pair<int, int>& state) const;


    int getStepCount() const;

    Environment* getEnvironment() { return environment; }

    std::pair<int, int> getNextState(const std::pair<int, int>& currentState, ActionType action);

    //从初始位置进行1次长度为length的episode
    std::vector<EpisodeStep> generateEpisode(Agent& agent, Environment* penv, std::pair<int, int> start, int length);
    //从初始位置进行n次长度为length的episode
    std::vector<std::vector<EpisodeStep>> generateEpisodes(Agent& agent, Environment* env, std::pair<int, int> start, int length, int numEpisodes);
    //从所有位置都进行n次长度为length的episode
    std::vector<std::vector<EpisodeStep>> iteratorGenerateEpisodes(Agent& agent, Environment* env,  int length, int numEpisodes);
};
