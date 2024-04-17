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
#include "batchGradientDescent.h"
#include "StochasticGradientDescent.h"
#include "Sarsa.h"
#include "nStepSarsa.h"
#include "QLearning.h"
#include <cstdlib> // ����rand()��srand()
#include <ctime> // ����time()

struct EpisodeStep {
    std::pair<int, int> state;
    ActionType action;
    float reward;
};


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

    // ��������Բ���ѡ����
    ActionType chooseActionStochastic(const std::pair<int, int>& state);


    
    // ��ȡĳ״̬������Բ���
    std::map<ActionType, float> getStochasticPolicy(const std::pair<int, int>& state) const;


    int getStepCount() const;

    Environment* getEnvironment() { return environment; }

    std::pair<int, int> getNextState(const std::pair<int, int>& currentState, ActionType action);

    //�ӳ�ʼλ�ý���1�γ���Ϊlength��episode
    std::vector<EpisodeStep> generateEpisode(Agent& agent, Environment* penv, std::pair<int, int> start, int length);
    //�ӳ�ʼλ�ý���n�γ���Ϊlength��episode
    std::vector<std::vector<EpisodeStep>> generateEpisodes(Agent& agent, Environment* env, std::pair<int, int> start, int length, int numEpisodes);
    //������λ�ö�����n�γ���Ϊlength��episode
    std::vector<std::vector<EpisodeStep>> iteratorGenerateEpisodes(Agent& agent, Environment* env,  int length, int numEpisodes);
};
