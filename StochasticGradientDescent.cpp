#include "StochasticGradientDescent.h"
#include "Agent.h"
#include <iostream>
#include <set>
#include <numeric>



void StochasticGradientDescent::evaluatePolicy(Agent& agent) {

    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();

    auto episodes = agent.iteratorGenerateEpisodes(agent, penv, EpisodeLen, EpisodeNum);
    auto& stateValues = penv->getStateValues();

    // ����gamma��alpha�Ѷ���
    std::map<std::pair<size_t, size_t>, std::pair<float, int>> stateGradients;

    for (const auto& episode : episodes) {
        float G = 0; // �ۻ�����
        for (auto it = episode.rbegin(); it != episode.rend(); ++it) {
            G = it->reward + gamma * G; // �����ۻ�����
            auto& state = it->state;
            auto& value = stateValues[state.first][state.second];
            float gradient = -2 * (G - value); // �����ݶ�

            // �ۼӵ�ǰ״̬���ݶȣ���������״̬���ֵĴ���
            if (stateGradients.find(state) == stateGradients.end()) {
                stateGradients[state] = std::make_pair(gradient, 1);
            }
            else {
                stateGradients[state].first += gradient;
                stateGradients[state].second += 1;
            }
        }
    }

    // ʹ���ۻ����ݶȺͼ���������״ֵ̬
    for (auto& [state, gradCount] : stateGradients) {
        auto& [cumulativeGradient, count] = gradCount;
        float avgGradient = cumulativeGradient / count; // ����ƽ���ݶ�
        stateValues[state.first][state.second] -= alpha * avgGradient; // ����״ֵ̬
    }
}

void StochasticGradientDescent::improvePolicyGreedy(Agent& agent) {

    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    for (size_t i = 0; i < penv->getGrid().size(); i++) {
        for (size_t j = 0; j < penv->getGrid()[i].size(); j++) {
            std::pair<int, int> state = { i, j };
            auto actionProbabilities = agent.getStochasticPolicy(state);
            float maxActionValue = -std::numeric_limits<float>::infinity();
            ActionType bestAction = ActionType::Up;
            for (const auto& [action, prob] : actionProbabilities) {
                //if (action == ActionType::Stay && grid[state.first][state.second] != CellType::Target)continue;
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


void StochasticGradientDescent::run(Agent& agent) {


    evaluatePolicy(agent);
    improvePolicyGreedy(agent);

}