#pragma once
#include <map>
#include <vector>
#include "Environment.h"
#include <cstdlib> // ����rand()��srand()
#include <ctime> // ����time()


class Agent {
private:
    // ȷ���Բ��ԣ�����ÿ��״̬���洢һ����Ѷ���
    std::map<std::pair<int, int>, ActionType> deterministicPolicy;

    // ����Բ��ԣ�����ÿ��״̬���洢ÿ�������ĸ���
    std::map<std::pair<int, int>, std::map<ActionType, float>> stochasticPolicy;

public:
    Agent() {
        srand(static_cast<unsigned int>(time(nullptr)));
        initializeDeterministicPolicy(5);
        initializeStochasticPolicy(5);
    };
    ~Agent() = default;

    // ����ȷ���Բ��Եķ���
    void setDeterministicPolicy(const std::pair<int, int>& state, ActionType action) {
        deterministicPolicy[state] = action;
    }

    void initializeDeterministicPolicy(int gridSize) {
        for (int x = 0; x < gridSize; x++) {
            for (int y = 0; y < gridSize; y++) {
                ActionType action = static_cast<ActionType>(rand() % 5); // ���ѡ����
                deterministicPolicy[{x, y}] = action;
            }
        }
    }

    void initializeStochasticPolicy(int gridSize) {
        for (int x = 0; x < gridSize; x++) {
            for (int y = 0; y < gridSize; y++) {
                std::map<ActionType, float> probabilities;
                float total = 0;
                for (int action = 0; action < 5; action++) {
                    float prob = static_cast<float>(rand()) / RAND_MAX; // ����0��1֮����������
                    probabilities[static_cast<ActionType>(action)] = prob;
                    total += prob;
                }
                // ��һ������
                for (auto& pair : probabilities) {
                    pair.second /= total;
                }
                stochasticPolicy[{x, y}] = probabilities;
            }
        }
    }

    // ��ȡȷ���Բ����µĶ���
    ActionType getActionForState(const std::pair<int, int>& state) {
        // �����״̬�ж��嶯�����򷵻ظö��������򣬷���Ĭ�϶���
        if (deterministicPolicy.find(state) != deterministicPolicy.end()) {
            return deterministicPolicy[state];
        }
        else {
            // ����һ��Ĭ�϶���������ͣ�����������ѡ��һ������
            return ActionType::Up; // ʾ����Ĭ������
        }
    }

    // ��������Բ��Եķ���
    void setStochasticPolicy(const std::pair<int, int>& state, const std::map<ActionType, float>& actionProbabilities) {
        stochasticPolicy[state] = actionProbabilities;
    }

    // ��������Բ���ѡ�����ķ�����α���룩
    ActionType chooseActionStochastic(const std::pair<int, int>& state) {
        // ����Ӧʵ�ָ��ݸ���ѡ�������߼�
        // ʾ������Ĭ�϶���
        return ActionType::Up;
    }

    // ��Agent����
    std::map<ActionType, float> getStochasticPolicy(const std::pair<int, int>& state) const {
        if (stochasticPolicy.find(state) != stochasticPolicy.end()) {
            return stochasticPolicy.at(state);
        }
        return std::map<ActionType, float>(); // ���״̬δ���壬�򷵻ؿյĲ���
    }

    // ����Agent����...
};
