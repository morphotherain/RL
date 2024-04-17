#include "Sarsa.h"
#include "Agent.h"
#include <iostream>

void Sarsa::run(Agent& agent)
{
    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    if (qTable.size() == 0)qTable.resize(penv->getGridSize(), std::vector<std::vector<float>>(penv->getGridSize(), std::vector<float>(5, 0.0f)));

    for (int episode = 0; episode < EpisodeNum; ++episode) {
        std::pair<int, int> state = { 0,0 };//penv->getInitialState(); // ��������������س�ʼ״̬
        ActionType action = agent.chooseActionStochastic(state); // ��������������ڵ�ǰ����ѡ����

        for (int step = 0; step < EpisodeLen; ++step) {
            auto s_next = agent.getNextState(state, action); // �����������ִ�ж�����������״̬�ͽ���
            auto reward = penv->getReward(state.first, state.second, action);
            ActionType nextAction = agent.chooseActionStochastic(s_next); // ѡ����һ������

            // SARSA����
            float& q = qTable[state.first][state.second][static_cast<int>(action)];
            float q_next = qTable[s_next.first][s_next.second][static_cast<int>(nextAction)];
            q += alpha * (reward + gamma * q_next - q); // ����agent.getAlpha()����ѧϰ��alpha

            state = s_next;
            action = nextAction;

            if (grid[state.first][state.second] == CellType::Target) break; // ��������������״̬�Ƿ�Ϊ��ֹ״̬
        }
    }
    improvePolicyGreedy(agent);
    evaluatePolicy(agent);
}

void Sarsa::improvePolicyGreedy(Agent& agent)
{
    auto penv = agent.getEnvironment();
    for (size_t i = 0; i < qTable.size(); ++i) {
        for (size_t j = 0; j < qTable[i].size(); ++j) {
            std::pair<int, int> state = { i, j };
            float maxQ = -std::numeric_limits<float>::infinity();
            ActionType bestAction = ActionType::Stay;
            for (int a = 0; a < 5; ++a) { // ������5������
                if (qTable[i][j][a] > maxQ) {
                    maxQ = qTable[i][j][a];
                    bestAction = static_cast<ActionType>(a);
                }
            }
            std::map<ActionType, float> policy;
            float �� = 0.1;
            for (int a = 0; a < 5; ++a) {
                policy[static_cast<ActionType>(a)] = (a == static_cast<int>(bestAction)) ? (1.0f - �� + (�� / 5.0f)) : (�� / 5.0f);
            }
            agent.setStochasticPolicy(state, policy);
        }
    }
}

void Sarsa::evaluatePolicy(Agent& agent)
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
