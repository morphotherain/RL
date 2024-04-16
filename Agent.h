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
#include "MC��Greedy.h"
#include "GradientDescent.h"
#include <cstdlib> // ����rand()��srand()
#include <ctime> // ����time()


class Agent {
private:
    Environment* environment;

    RLAlgorithm* algorithm = nullptr;

    // ����Բ��ԣ�����ÿ��״̬���洢ÿ�������ĸ���
    std::map<std::pair<int, int>, std::map<ActionType, float>> stochasticPolicy;

    std::map<ActionType, std::pair<int, int>> actionSpace; // �����ռ�

    int stepCount = 0; // ������


public:
    Agent(Environment* env);
    ~Agent();

    void setAlgorithm(RLAlgorithm* newAlgorithm);

    void run();

    void initializeDeterministicPolicy(Environment* env);

    void initializeStochasticPolicy(Environment* env);

    void initializeActionSpace(); // ��ʼ�������ռ�

    std::map<ActionType, std::pair<int, int>> getActionSpace();


    // ��������Բ��Եķ���
    void setStochasticPolicy(const std::pair<int, int>& state, const std::map<ActionType, float>& actionProbabilities);

    // ��������Բ���ѡ������α���룩
    ActionType chooseActionStochastic(const std::pair<int, int>& state);
    
    // ��ȡĳ״̬������Բ���
    std::map<ActionType, float> getStochasticPolicy(const std::pair<int, int>& state) const;


    int getStepCount() const;

    Environment* getEnvironment() { return environment; }

    std::pair<int, int> getNextState(const std::pair<int, int>& currentState, ActionType action);
};
