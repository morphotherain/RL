#include "MCBasic.h"
#include "Agent.h"
#include <iostream>
#include <set>
#include <numeric>

struct EpisodeStep {
    std::pair<int, int> state;
    ActionType action;
    float reward;
};

std::vector<EpisodeStep> generateEpisode(Agent& agent, Environment* penv, std::pair<int, int> start, int length) {
    std::vector<EpisodeStep> episode;
    std::pair<int,int> state = start;
    const auto& grid = penv->getGrid();
    while (length>0 && grid[state.first][state.second] != CellType::Target) {
        auto actionProbabilities = agent.getStochasticPolicy(state);
        ActionType action = agent.chooseActionStochastic(state); // ��Ҫʵ�ָ��ݸ���ѡ�����ķ���
        auto s_next = agent.getNextState(state, action);
        auto reward = penv->getReward(state.first,state.second, action);
        episode.push_back({ state, action, reward });
        state = s_next;
        length--;
    }
    return episode;
}

void evaluatePolicy(Agent& agent, Environment* penv, int numEpisodes, float gamma, int lenEpisodes) {
    std::map<std::pair<int, int>, std::vector<float>> returns; // ״̬�����з���ֵ
    const auto& grid = penv->getGrid();
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            for (int e = 0; e < numEpisodes; ++e) {
                auto episode = generateEpisode(agent, penv, {i,j}, lenEpisodes);
                std::set<std::pair<int, int>> visitedStates; // ���ڼ�¼������ѷ��ʵ�״̬
                float G = 0; // ��ʼ���ۻ�����
                for (auto it = episode.rbegin(); it != episode.rend(); ++it) { // ����������
                    G = it->reward + gamma * G; // �����ۻ�����
                    auto& state = it->state;
                    if (visitedStates.count(state) == 0 && (it + 1) == episode.rend()) { // ������״η���
                        visitedStates.insert(state);
                        returns[state].push_back(G);
                    }
                }
            }
        }
    }

    // ����״ֵ̬Ϊ���з���ֵ��ƽ��ֵ
    auto& stateValues = penv->getStateValues();
    for (auto& [state, allReturns] : returns) {
        float averageReturn = std::accumulate(allReturns.begin(), allReturns.end(), 0.0f) / allReturns.size();
        stateValues[state.first][state.second] = averageReturn;
    }
}

void improvePolicyGreedy(Agent& agent, Environment* penv,float gamma) {
    const auto& grid = penv->getGrid();
    for (size_t i = 0; i < penv->getGrid().size(); i++) {
        for (size_t j = 0; j < penv->getGrid()[i].size(); j++) {
            std::pair<int, int> state = { i, j };
            auto actionProbabilities = agent.getStochasticPolicy(state);
            float maxActionValue = -std::numeric_limits<float>::infinity();
            ActionType bestAction = ActionType::Up;
            for (const auto& [action, prob] : actionProbabilities){
                if (action == ActionType::Stay && grid[state.first][state.second] != CellType::Target)continue;
                auto s_next = agent.getNextState(state, action);
                float reward = penv->getReward(state.first, state.second, action);
                float actionValue = reward + gamma * penv->getStateValues()[s_next.first][s_next.second];
                if (actionValue > maxActionValue) {
                    maxActionValue = actionValue;
                    bestAction = action;
                }
            }
            // ���²��ԣ�ֻΪ��Ѷ������ø���Ϊ1����������Ϊ0
            std::map<ActionType, float> updatedProbabilities;
            for (const auto& action : actionProbabilities) {
                updatedProbabilities[action.first] = (action.first == bestAction) ? 1.0f : 0.0f;
            }
            agent.setStochasticPolicy(state, updatedProbabilities);
        }
    }
}


void MCBasic::run(Agent& agent) {

    auto gamma = 0.9f;

    auto penv = agent.getEnvironment();
    evaluatePolicy(agent, penv, EpisodeNum, gamma, EpisodeLen);
    improvePolicyGreedy(agent, penv,  gamma);

}