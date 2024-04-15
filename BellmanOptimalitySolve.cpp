#include "BellmanOptimalitySolve.h"
#include "Agent.h"
#include <iostream>

void BellmanOptimalitySolve::run(Agent& agent) {
    std::cout << "BellmanOptimalitySolve run" << std::endl;

    auto gamma = 0.9f;

    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    auto& stateValues = penv->getStateValues();


    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {

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