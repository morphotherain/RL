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

    // �������Ŀ������
    int targetX = rand() % gridSize;
    int targetY = rand() % gridSize;
    grid[targetX][targetY] = CellType::Target;

    // �������5���ϰ���
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
    // ���Ը�����Ҫ��ʼ���ض����ӵ�״ֵ̬
}

void Environment::initializeActionSpace() {
    actionSpace[ActionType::Up] = { 0, -1 };
    actionSpace[ActionType::Down] = { 0, 1 };
    actionSpace[ActionType::Left] = { -1, 0 };
    actionSpace[ActionType::Right] = { 1, 0 };
    actionSpace[ActionType::Right] = { 0, 0 };
}


float Environment::getReward(int x, int y, ActionType action) {
    // �����ƶ����λ��
    int newX = x + actionSpace[action].first;
    int newY = y + actionSpace[action].second;

    // ���߽�
    if (newX < 0 || newX >= gridSize || newY < 0 || newY >= gridSize) {
        return -1.0f; // ����ͷ�
    }

    // ���ݸ������ͷ��ؽ���
    switch (grid[newX][newY]) {
    case CellType::Target:
        return 10.0f; // �ﵽĿ��Ľ���
    case CellType::Obstacle:
        return -10.0f; // �����ϰ��ĳͷ�
    default:
        return 0.0f; // һ���ƶ��ĳͷ�������̽��
    }
}