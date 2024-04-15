#include "TruncatePolicyIteration.h"
#include "Agent.h"
#include <iostream>

void TruncatePolicyIteration::run(Agent& agent) {
    std::cout << "BellmanOptimalitySolve run" << std::endl;

    auto gamma = 0.9f;

    auto TruncateNum = 6;

    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    auto& stateValues = penv->getStateValues();


    for(; TruncateNum>0; TruncateNum--)
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {

            auto newStateValue = 0.0f;

            // 获取当前格子的策略
            auto actionProbabilities = agent.getStochasticPolicy({ i, j });

            for (auto& action : actionProbabilities) {
                auto s_next = agent.getNextState({ i,j }, action.first);

                auto reward = penv->getReward(i, j, action.first);
                newStateValue += reward * action.second;
                newStateValue += stateValues[s_next.first][s_next.second] * action.second * gamma;

            }

            stateValues[i][j] = newStateValue;

        }
    }


    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            auto stateType = (grid[i][j]);

            auto OptimalityActionValue = -INFINITY;
            auto OptimalityAction = ActionType::Stay;

            // 获取当前格子的策略
            auto actionProbabilities = agent.getStochasticPolicy({ i, j });

            for (auto& action : actionProbabilities) {
                auto actionValue = 0.0f;

                auto s_next = agent.getNextState({ i,j }, action.first);

                auto reward = penv->getReward(i, j, action.first);
                actionValue += reward;
                actionValue += stateValues[s_next.first][s_next.second] * gamma;

                if (stateValues[i][j] < 1.0f)
                    std::cout << actionValue << std::endl;

                if (actionValue > OptimalityActionValue)
                {
                    OptimalityAction = action.first;
                    OptimalityActionValue = actionValue;
                }

            }
            for (int a = 0; a < 5; a++) {
                actionProbabilities[static_cast<ActionType>(a)] = 0.0f;
            };
            actionProbabilities[(OptimalityAction)] = 1.0f;
            agent.setStochasticPolicy({ i,j }, actionProbabilities);

            stateValues[i][j] = OptimalityActionValue;

        }
    }
}