#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <vector>
#include <utility> // For std::pair
#include <map>
#include <vector>

enum class CellType { Empty, Obstacle, Target };

enum class ActionType { Up, Down, Left, Right, Stay };

class Environment {
private:
    int gridSize = 5;
    std::vector<std::vector<CellType>> grid;
    std::vector<std::vector<float>> stateValues; // 状态值存储
    std::map<ActionType, std::pair<int, int>> actionSpace; // 动作空间
public:
    Environment();
    void initializeGrid();
    void initializeStateValues(); // 初始化状态值
    void initializeActionSpace(); // 初始化动作空间
    float getReward(int x, int y, ActionType action); // 计算动作奖励
    const std::vector<std::vector<CellType>>& getGrid() const { return grid; }

    // 在Environment类中
    std::vector<std::vector<float>>& getStateValues() {
        return stateValues;
    }

};

#endif
