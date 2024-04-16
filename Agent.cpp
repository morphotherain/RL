#include "Agent.h"
#include <random> // ����������������ͷֲ�

Agent::Agent(Environment* env) : environment(env) {
    srand(static_cast<unsigned int>(time(nullptr)));
    initializeDeterministicPolicy(env);
    initializeStochasticPolicy(env);
    //initializeActionSpace();
    setAlgorithm(new StochasticGradientDescent()); // Ĭ������ΪBellmanSolve�㷨
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



void Agent::initializeDeterministicPolicy(Environment* env) {
    const auto& grid = env->getGrid();
    for (int x = 0; x < grid.size(); x++) {
        for (int y = 0; y < grid[x].size(); y++) {
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

void Agent::initializeStochasticPolicy(Environment* env) {
    const auto& grid = env->getGrid();
    for (int x = 0; x < grid.size(); x++) {
        for (int y = 0; y < grid[x].size(); y++) {
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



std::vector<EpisodeStep> Agent::generateEpisode(Agent & agent, Environment * penv, std::pair<int, int> start, int length) {
    std::vector<EpisodeStep> episode;
    std::pair<int, int> state = start;
    const auto& grid = penv->getGrid();
    while (length > 0 && grid[state.first][state.second] != CellType::Target) {
        auto actionProbabilities = agent.getStochasticPolicy(state);
        ActionType action = agent.chooseActionStochastic(state);
        auto s_next = agent.getNextState(state, action);
        auto reward = penv->getReward(state.first, state.second, action);
        episode.push_back({ state, action, reward });
        state = s_next;
        length--;
    }
    return episode;
}

std::vector<std::vector<EpisodeStep>> Agent::generateEpisodes(Agent& agent, Environment* env, std::pair<int, int> start, int length, int numEpisodes) {
    std::vector<std::vector<EpisodeStep>> episodes;
    for (int episode = 0; episode < numEpisodes; ++episode) {
        episodes.push_back(generateEpisode(agent, env, start, length));
    }
    return episodes;
}

std::vector<std::vector<EpisodeStep>> Agent::iteratorGenerateEpisodes(Agent& agent, Environment* env, int length, int numEpisodes)
{
    std::vector<std::vector<EpisodeStep>> episodes;

    auto penv = agent.getEnvironment();
    const auto& grid = penv->getGrid();
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            for (int episode = 0; episode < numEpisodes; ++episode) {
                episodes.push_back(generateEpisode(agent, env, {i,j}, length));
            }
        }
    }

    return episodes;
}
