#include "Environment.h"
#include <cstdlib> // For rand() and srand()
#include <ctime> // For time()

Environment::Environment() {
    srand(time(nullptr)); // Seed the random number generator
    initializeGrid();
    initializeStateValues();
    initializeActionSpace();
}

void Environment::initializeGrid() {
    grid = std::vector<std::vector<CellType>>(gridSize, std::vector<CellType>(gridSize, CellType::Empty));

    // 随机放置目标区域
    int targetX = rand() % gridSize;
    int targetY = rand() % gridSize;
    grid[targetX][targetY] = CellType::Target;

    // 随机放置5个障碍物
    for (int i = 0; i < 5; ) {
        int obsX = rand() % gridSize;
        int obsY = rand() % gridSize;
        if (grid[obsX][obsY] == CellType::Empty) {
            grid[obsX][obsY] = CellType::Obstacle;
            i++;
        }
    }
}


void Environment::initializeStateValues() {
    stateValues = std::vector<std::vector<float>>(gridSize, std::vector<float>(gridSize, 0.0f));
    // 可以根据需要初始化特定格子的状态值
}

void Environment::initializeActionSpace() {
    actionSpace[ActionType::Up] = { 0, -1 };
    actionSpace[ActionType::Down] = { 0, 1 };
    actionSpace[ActionType::Left] = { -1, 0 };
    actionSpace[ActionType::Right] = { 1, 0 };
    actionSpace[ActionType::Right] = { 0, 0 };
}


float Environment::getReward(int x, int y, ActionType action) {
    // 假设移动后的位置
    int newX = x + actionSpace[action].first;
    int newY = y + actionSpace[action].second;

    // 检查边界
    if (newX < 0 || newX >= gridSize || newY < 0 || newY >= gridSize) {
        return -1.0f; // 出界惩罚
    }

    // 根据格子类型返回奖励
    switch (grid[newX][newY]) {
    case CellType::Target:
        return 10.0f; // 达到目标的奖励
    case CellType::Obstacle:
        return -10.0f; // 碰到障碍的惩罚
    default:
        return 0.0f; // 一般移动的惩罚，鼓励探索
    }
}