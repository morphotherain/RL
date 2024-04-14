#pragma once
#include <map>
#include <vector>
#include "Environment.h"
#include <cstdlib> // 包含rand()和srand()
#include <ctime> // 包含time()


class Agent {
private:
    // 确定性策略：对于每个状态，存储一个最佳动作
    std::map<std::pair<int, int>, ActionType> deterministicPolicy;

    // 随机性策略：对于每个状态，存储每个动作的概率
    std::map<std::pair<int, int>, std::map<ActionType, float>> stochasticPolicy;

public:
    Agent() {
        srand(static_cast<unsigned int>(time(nullptr)));
        initializeDeterministicPolicy(5);
        initializeStochasticPolicy(5);
    };
    ~Agent() = default;

    // 设置确定性策略的方法
    void setDeterministicPolicy(const std::pair<int, int>& state, ActionType action) {
        deterministicPolicy[state] = action;
    }

    void initializeDeterministicPolicy(int gridSize) {
        for (int x = 0; x < gridSize; x++) {
            for (int y = 0; y < gridSize; y++) {
                ActionType action = static_cast<ActionType>(rand() % 5); // 随机选择动作
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
                    float prob = static_cast<float>(rand()) / RAND_MAX; // 生成0到1之间的随机概率
                    probabilities[static_cast<ActionType>(action)] = prob;
                    total += prob;
                }
                // 归一化概率
                for (auto& pair : probabilities) {
                    pair.second /= total;
                }
                stochasticPolicy[{x, y}] = probabilities;
            }
        }
    }

    // 获取确定性策略下的动作
    ActionType getActionForState(const std::pair<int, int>& state) {
        // 如果该状态有定义动作，则返回该动作；否则，返回默认动作
        if (deterministicPolicy.find(state) != deterministicPolicy.end()) {
            return deterministicPolicy[state];
        }
        else {
            // 返回一个默认动作，比如停留不动或随机选择一个动作
            return ActionType::Up; // 示例：默认向上
        }
    }

    // 设置随机性策略的方法
    void setStochasticPolicy(const std::pair<int, int>& state, const std::map<ActionType, float>& actionProbabilities) {
        stochasticPolicy[state] = actionProbabilities;
    }

    // 根据随机性策略选择动作的方法（伪代码）
    ActionType chooseActionStochastic(const std::pair<int, int>& state) {
        // 这里应实现根据概率选择动作的逻辑
        // 示例返回默认动作
        return ActionType::Up;
    }

    // 在Agent类中
    std::map<ActionType, float> getStochasticPolicy(const std::pair<int, int>& state) const {
        if (stochasticPolicy.find(state) != stochasticPolicy.end()) {
            return stochasticPolicy.at(state);
        }
        return std::map<ActionType, float>(); // 如果状态未定义，则返回空的策略
    }

    // 其他Agent方法...
};
