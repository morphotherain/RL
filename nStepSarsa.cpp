#include "nStepSarsa.h"
#include "Agent.h"
#include <iostream>
#include <numeric>
#include <deque>

void nStepSarsa::run(Agent& agent)
{
    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    if (qTable.size() == 0) {
        qTable.resize(penv->getGridSize(), std::vector<std::vector<float>>(penv->getGridSize(), std::vector<float>(5, 10)));
    }

    int n = stepNum; // n步SARSA

    for (int episode = 0; episode < EpisodeNum; ++episode) {
        std::pair<int, int> state = { 0, 0 }; // 假设getInitialState返回初始状态
        ActionType action = agent.chooseActionStochastic(state);
        std::cout << static_cast<int>(action) << std::endl;

        std::deque<std::tuple<std::pair<int, int>, ActionType, float>> nStepMemory; // 用于存储n步的状态、动作和奖励

        for (int step = 0; step < EpisodeLen + n; ++step) {
            std::pair<int, int> s_next = state;
            float reward = 0.0;
            ActionType nextAction = action;

            if (step < EpisodeLen) {
                s_next = agent.getNextState(state, action); // 执行动作并返回新状态
                reward = penv->getReward(state.first, state.second, action);
                nextAction = agent.chooseActionStochastic(s_next); // 选择下一个动作
                nStepMemory.push_back(std::make_tuple(state, action, reward)); // 存储到n-step记忆中
            }

            if (step >= n - 1 || grid[s_next.first][s_next.second] == CellType::Target) {
                if (!nStepMemory.empty()) {
                    // 更新Q值
                    auto [initState, initAction, _] = nStepMemory.front();
                    float& q = qTable[initState.first][initState.second][static_cast<int>(initAction)];
                    float totalReward = std::accumulate(nStepMemory.begin(), nStepMemory.end(), 0.0f,
                        [](float sum, const auto& item) { return sum + std::get<2>(item); });

                    if (step < EpisodeLen) {
                        float q_next = qTable[s_next.first][s_next.second][static_cast<int>(nextAction)];
                        q += alpha * (totalReward + std::pow(gamma, std::min(n, static_cast<int>(nStepMemory.size()))) * q_next - q);
                    }
                    else {
                        // 在序列结束时只累积剩余的奖励
                        q += alpha * (totalReward - q);
                    }

                    nStepMemory.pop_front(); // 移除最旧的一步
                }
            }

            if (grid[s_next.first][s_next.second] == CellType::Target) break; // 检查是否终止
            state = s_next;
            action = nextAction;
        }
    }


    improvePolicyGreedy(agent);
    evaluatePolicy(agent);
}

void nStepSarsa::improvePolicyGreedy(Agent& agent)
{
    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    for (size_t i = 0; i < qTable.size(); ++i) {
        for (size_t j = 0; j < qTable[i].size(); ++j) {
            std::cout << "[i][j]:  " << i <<" , " << j << std::endl;
            std::pair<int, int> state = { i, j };
            float maxQ = -std::numeric_limits<float>::infinity();
            ActionType bestAction = ActionType::Stay;
            for (int a = 0; a < 4; ++a) { // 假设有5个动作
                std::cout << "qTable[i][j][a]:  " << qTable[i][j][a] << std::endl;
                if (qTable[i][j][a] > maxQ) {
                    maxQ = qTable[i][j][a];
                    bestAction = static_cast<ActionType>(a);
                }
            }
            std::map<ActionType, float> policy;
            float ε = 0.2;
            std::cout <<"bestAction:  " << static_cast<int>(bestAction) << std::endl;
            for (int a = 0; a < 5; ++a) {
                if (grid[state.first][state.second] == CellType::Target)
                {
                    policy[static_cast<ActionType>(a)] = (a == static_cast<int>(ActionType::Stay)) ? (1.0f) : ( 0.0f);             
                }
                else
                {
                    if (a == 4) policy[static_cast<ActionType>(a)] = 0.0f;
                    else
                    {
                        policy[static_cast<ActionType>(a)] = (bestAction == ActionType::Stay) ? 0.25
                            : ((a == static_cast<int>(bestAction)) ? (1.0f - ε + (ε / 4.0f))
                                : (ε / 4.0f));
                    }
                }
            }
            agent.setStochasticPolicy(state, policy);
        }
    }
}

void nStepSarsa::evaluatePolicy(Agent& agent)
{
    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    auto& stateValues = penv->getStateValues();


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
}
