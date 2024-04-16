#include "GradientDescent.h"
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


void GDevaluatePolicy(Agent& agent, Environment* penv, float gamma, float alpha) {
    const auto& grid = penv->getGrid();


    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            std::pair<int, int> state = { i, j };

            ActionType action = agent.chooseActionStochastic(state);
            // 假设getNextState和getReward能够处理边界情况
            auto s_next = agent.getNextState(state, action);
            auto reward = penv->getReward(i, j, action);

            // 计算目标价值
            float target = reward + gamma * penv->getStateValues()[s_next.first][s_next.second];
            float value = penv->getStateValues()[i][j];

            // 计算梯度并更新状态值
            float gradient = -2 * (target - value);
            penv->getStateValues()[i][j] -= alpha * gradient;
        }
    }
}

void GDimprovePolicyGreedy(Agent& agent, Environment* penv, float gamma) {
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
            // 更新策略，只为最佳动作设置概率为1，其他动作为0
            std::map<ActionType, float> updatedProbabilities;
            for (const auto& action : actionProbabilities) {
                updatedProbabilities[action.first] = (action.first == bestAction) ? 1.0f : 0.0f;
            }
            agent.setStochasticPolicy(state, updatedProbabilities);
        }
    }
}


void GradientDescent::run(Agent& agent) {

    
    auto penv = agent.getEnvironment();
    GDevaluatePolicy(agent, penv, gamma, alpha);
    GDimprovePolicyGreedy(agent, penv, gamma);

}