#include "QLearning.h"
#include <iostream>
#include <numeric>
#include <deque>
#include <algorithm>
#include <random>

ActionType QLearning::chooseActionStochasticWithEpsilon(const std::pair<int, int>& state)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    if (dis(gen) < epsilon) { // �Ԧŵĸ������ѡ��һ������
        std::uniform_int_distribution<> actionDis(0, 4); // ������4������, ����stay
        return static_cast<ActionType>(actionDis(gen));
    }
    else { // ��(1 - ��)�ĸ���ѡ�����Ŷ���
        auto& actions = qTable[state.first][state.second];
        int bestAction = std::distance(actions.begin(), std::max_element(actions.begin(), actions.end()-1));
        return static_cast<ActionType>(bestAction);
    }
    return ActionType();
}

void QLearning::run(Agent& agent)
{
    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    // ȷ��Q���Ѿ���ʼ��
    if (qTable.size() == 0) {
        qTable.resize(penv->getGridSize(), std::vector<std::vector<float>>(penv->getGridSize(), std::vector<float>(5, 1.0f))); // ������5������
    }

    for (int episode = 0; episode < EpisodeNum; ++episode) {
        std::pair<int, int> state = { 0, 0 };

        for (int len = 0; len < EpisodeLen; ++len) { // һ��episode��ѭ��
            std::cout << "len:" << len << "x, y " << state.first << " , " << state.second << std::endl;
            ActionType action = chooseActionStochasticWithEpsilon(state); // ���ݵ�ǰ����ѡ����
            auto s_next = agent.getNextState(state, action); // ִ�ж�����������״̬
            auto reward = penv->getReward(state.first, state.second, action); // ��ý���

            // Q-learning���ĸ����߼�
            float& q = qTable[state.first][state.second][static_cast<int>(action)];
            float maxQNext = *std::max_element(qTable[s_next.first][s_next.second].begin(), qTable[s_next.first][s_next.second].end());
            q += alpha * (reward + gamma * maxQNext - q);

            state = s_next; // ������һ��״̬

            if (grid[s_next.first][s_next.second] == CellType::Target) break; // ����ﵽ�յ㣬�������ǰepisode
        }

        decayEpsilon(); // ��episode����ʱ��С��

        // ��ÿ��episode����ʱ���²���
        for (size_t i = 0; i < qTable.size(); ++i) {
            for (size_t j = 0; j < qTable[i].size(); ++j) {
                std::pair<int, int> s = { i, j };
                int bestActionIndex = std::distance(qTable[i][j].begin(), std::max_element(qTable[i][j].begin(), qTable[i][j].end()));
                ActionType bestAction = static_cast<ActionType>(bestActionIndex);

                // ���²���Ϊѡ��Qֵ���Ķ���
                std::map<ActionType, float> actionProbabilities;
                for (int a = 0; a < 5; ++a) { // ������5������
                    actionProbabilities[static_cast<ActionType>(a)] = (a == bestActionIndex) ? 1.0f : 0.0f;
                }
                agent.setStochasticPolicy(s, actionProbabilities);
            }
        }
    }
    evaluatePolicy(agent);
}

void QLearning::evaluatePolicy(Agent& agent)
{
    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    auto& stateValues = penv->getStateValues();


    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {

            auto newStateValue = 0.0f;

            // ��ȡ��ǰ���ӵĲ���
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