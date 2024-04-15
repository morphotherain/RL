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
    std::vector<std::vector<float>> stateValues; // ״ֵ̬�洢
    std::map<ActionType, std::pair<int, int>> actionSpace; // �����ռ�
public:
    Environment();
    void initializeGrid();
    void initializeStateValues(); // ��ʼ��״ֵ̬
    void initializeActionSpace(); // ��ʼ�������ռ�
    float getReward(int x, int y, ActionType action); // ���㶯������
    const std::vector<std::vector<CellType>>& getGrid() const { return grid; }

    // ��Environment����
    std::vector<std::vector<float>>& getStateValues() {
        return stateValues;
    }

};

#endif
