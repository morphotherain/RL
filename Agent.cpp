#include "Agent.h"
#include <random> // ����������������ͷֲ�

Agent::Agent(Environment* env) : environment(env) {
    srand(static_cast<unsigned int>(time(nullptr)));
    initializeDeterministicPolicy(5);
    initializeStochasticPolicy(5);
    //initializeActionSpace();
    setAlgorithm(new TruncatePolicyIteration()); // Ĭ������ΪBellmanSolve�㷨
}

Agent::~Agent() {
    delete algorithm;
}

void Agent::setAlgorithm(RLAlgorithm* newAlgorithm) {
    delete algorithm;
    algorithm = newAlgorithm;
}

void Agent::run() {
    if (algorithm) {
        algorithm->run(*this); // �������ݸ��㷨
        stepCount++; // ��ÿ�������㷨ʱ�����������
    }
}



void Agent::initializeDeterministicPolicy(int gridSize) {
    for (int x = 0; x < gridSize; x++) {
        for (int y = 0; y < gridSize; y++) {
            std::map<ActionType, float> probabilities;
            for (int i = 0; i < 5; i++) {
                probabilities[static_cast<ActionType>(i)] = 0.0f;
            };
            int action = rand()%5;
            probabilities[static_cast<ActionType>(action)] = 1.0f;
            stochasticPolicy[{x, y}] = probabilities;
        }
    }
}

void Agent::initializeStochasticPolicy(int gridSize) {
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


// ��������Բ��Եķ���
void Agent::setStochasticPolicy(const std::pair<int, int>& state, const std::map<ActionType, float>& actionProbabilities) {
    stochasticPolicy[state] = actionProbabilities;
}

// ��������Բ���ѡ����
ActionType Agent::chooseActionStochastic(const std::pair<int, int>& state) {
    auto it = stochasticPolicy.find(state);
    if (it == stochasticPolicy.end()) {
        return ActionType::Stay; // ���״̬δ���壬����Ĭ�϶���
    }

    // �ۻ����ʱ߽�
    std::vector<std::pair<float, ActionType>> cumulativeProbabilities;
    float cumulative = 0.0;
    for (const auto& actionProb : it->second) {
        cumulative += actionProb.second;
        cumulativeProbabilities.push_back({ cumulative, actionProb.first });
    }

    // ����һ�������
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    float randomValue = dis(gen);

    // ���������ѡ����
    for (const auto& cp : cumulativeProbabilities) {
        if (randomValue <= cp.first) {
            return cp.second;
        }
    }

    return ActionType::Stay; // Ӧ����Զ���ᵽ��������Ǹ���֮�Ͳ�Ϊ1
}

std::map<ActionType, float> Agent::getStochasticPolicy(const std::pair<int, int>& state) const {
    if (stochasticPolicy.find(state) != stochasticPolicy.end()) {
        return stochasticPolicy.at(state);
    }
    return std::map<ActionType, float>(); // ���״̬δ���壬�򷵻ؿյĲ���
}


int Agent::getStepCount() const {
    return stepCount; // �ṩ��ȡ��ǰ��������ķ���
}

std::pair<int, int> Agent::getNextState(const std::pair<int, int>& currentState, ActionType action)
{
    const auto& grid = environment->getGrid();

    int dx = 0;
    int dy = 0;
    switch (action) {
    case ActionType::Up:
        dy = -1;
        break;
    case ActionType::Down:
        dy = 1;
        break;
    case ActionType::Left:
        dx = -1;
        break;
    case ActionType::Right:
        dx = 1;
        break;
    case ActionType::Stay:
    default:
        // ���ƶ�
        break;
    }
    // ������һ��״̬
    int nextX = std::max(0, std::min(static_cast<int>(grid.size()) - 1, currentState.first + dx));
    int nextY = std::max(0, std::min(static_cast<int>(grid[0].size()) - 1, currentState.second + dy));

    // �����һ��״̬�����߽磬�򱣳�ԭ�ز���
    if (nextX == currentState.first && nextY == currentState.second) {
        return currentState;
    }

    return { nextX, nextY };
}

void Agent::initializeActionSpace() {
    actionSpace[ActionType::Up] = { 0, -1 };
    actionSpace[ActionType::Down] = { 0, 1 };
    actionSpace[ActionType::Left] = { -1, 0 };
    actionSpace[ActionType::Right] = { 1, 0 };
    actionSpace[ActionType::Right] = { 0, 0 };
}

std::map<ActionType, std::pair<int, int>> Agent::getActionSpace()
{
    return actionSpace;
}

