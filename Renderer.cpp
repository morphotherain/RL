#include "Renderer.h"
#include <GL/glut.h>


#include <cmath> // For cos and sin functions

// 辅助函数：根据状态值选择颜色
void setColorBasedOnValue(float value) {
    // 示例：使用简单的线性映射将状态值映射到蓝色到红色的渐变
    float normalizedValue = std::min(std::max(value, 0.0f), 1.0f); // 确保值在0到1之间
    glColor3f(normalizedValue, 0.0, 1.0 - normalizedValue);
}

// 辅助函数：在指定位置绘制文本
void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y); // 设置文本开始绘制的位置
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c); // 使用固定宽度的字体绘制文本
    }
}

//// 辅助函数：绘制代表策略的线段或圆形
//void drawPolicyIndicator(float x, float y, float cellWidth, float cellHeight, const std::map<ActionType, float>& actionProbabilities) {
//    // 动作到方向的映射
//    std::map<ActionType, std::pair<float, float>> directions = {
//        {ActionType::Up, {0, -1}},
//        {ActionType::Down, {0, 1}},
//        {ActionType::Left, {-1, 0}},
//        {ActionType::Right, {1, 0}}
//    };
//
//    for (const auto& actionProbability : actionProbabilities) {
//        auto direction = directions[actionProbability.first];
//        float length = std::sqrt(actionProbability.second) * std::min(cellWidth, cellHeight) / 2; // 使用概率的平方根来缩放长度
//
//        // 如果策略是原地不动，则绘制一个圆形表示概率
//        if (actionProbability.first == ActionType::Stay) {
//            float radius = length/2.0f; // 使用概率来确定圆的半径
//            glBegin(GL_LINE_LOOP);
//            for (int i = 0; i < 360; i++) {
//                float degInRad = i * (3.14159f / 180.0f);
//                glVertex2f(cos(degInRad) * radius + x + cellWidth / 2, sin(degInRad) * radius + y + cellHeight / 2);
//            }
//            glEnd();
//        }
//        else {
//            // 否则，根据概率绘制线段
//            glBegin(GL_LINES);
//            glVertex2f(x + cellWidth / 2, y + cellHeight / 2);
//            glVertex2f(x + cellWidth / 2 + direction.first * length, y + cellHeight / 2 + direction.second * length);
//            glEnd();
//        }
//    }
//}

void drawPolicyIndicator(float x, float y, float cellWidth, float cellHeight, const std::map<ActionType, float>& actionProbabilities) {
    // 动作到方向的映射
    std::map<ActionType, std::pair<float, float>> directions = {
        {ActionType::Up, {0, -1}},
        {ActionType::Down, {0, 1}},
        {ActionType::Left, {-1, 0}},
        {ActionType::Right, {1, 0}}
    };

    for (const auto& actionProbability : actionProbabilities) {
        auto direction = directions[actionProbability.first];
        float length = std::sqrt(actionProbability.second) * std::min(cellWidth, cellHeight) / 2; // 使用概率的平方根来缩放长度

        // 根据概率设置颜色，从蓝色（低概率）到红色（高概率）
        float colorIntensity = actionProbability.second; // 假设概率在0到1之间
        glColor3f(colorIntensity, 0.0, 1.0 - colorIntensity);

        // 如果策略是原地不动，则绘制一个圆周表示概率
        if (actionProbability.first == ActionType::Stay) {
            float radius = length / 3.0f; // 使用概率来确定圆的半径
            glBegin(GL_LINE_LOOP); // 修改为GL_LINE_LOOP绘制空心圆
            for (int i = 0; i < 360; i++) {
                float degInRad = i * (3.14159f / 180.0f);
                glVertex2f(cos(degInRad) * radius + x + cellWidth / 2, sin(degInRad) * radius + y + cellHeight / 2);
            }
            glEnd();
        }
        else {
            // 否则，根据概率绘制线段
            glBegin(GL_LINES);
            glVertex2f(x + cellWidth / 2, y + cellHeight / 2);
            glVertex2f(x + cellWidth / 2 + direction.first * length, y + cellHeight / 2 + direction.second * length);
            glEnd();
        }
    }
}



void Renderer::drawEnvironment() {
    const auto& grid = environment->getGrid();
    const auto& stateValues = environment->getStateValues();
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            switch (grid[i][j]) {
            case CellType::Empty:
                glColor3f(1.0, 1.0, 1.0); // White
                break;
            case CellType::Obstacle:
                glColor3f(0.5, 0.5, 0.5); // Gray
                break;
            case CellType::Target:
                glColor3f(1.0, 0.0, 0.0); // Red
                break;
            }
            // Calculate cell coordinates
            float x = i * windowWidth / grid.size();
            float y = j * windowHeight / grid[i].size();
            float cellWidth = windowWidth / grid.size();
            float cellHeight = windowHeight / grid[i].size();
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + cellWidth, y);
            glVertex2f(x + cellWidth, y + cellHeight);
            glVertex2f(x, y + cellHeight);
            glEnd();
        }
    }
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            float x = i * windowWidth / grid.size();
            float y = j * windowHeight / grid[i].size();
            float cellWidth = windowWidth / grid.size();
            float cellHeight = windowHeight / grid[i].size();

            // 设置基于状态值的颜色
            setColorBasedOnValue(stateValues[i][j]);

            // 绘制状态值
            float stateValue = stateValues[i][j];
            char text[10];
            sprintf_s(text, "%.2f", stateValue); // 将状态值转换为字符串，保留两位小数

            // 计算文本绘制的起始位置，使其位于格子中心
            float textWidth = glutBitmapLength(GLUT_BITMAP_8_BY_13, (const unsigned char*)text);
            float textX = x + (cellWidth - textWidth) / 2.0f - 20.0f;
            float textY = y + cellHeight / 2.0f - 10.0f; // GLUT_BITMAP_8_BY_13的字符高度大约为13像素
            setColorBasedOnValue(stateValue); // 可以根据需要调整文本颜色
            drawText(text, textX, textY);

            // 获取当前格子的策略
            auto actionProbabilities = agent->getStochasticPolicy({ i, j });

            // 绘制策略指示符
            drawPolicyIndicator(x, y, cellWidth, cellHeight, actionProbabilities);
        }
    }
}


void Renderer::drawAgent() {
    // 使用OpenGL绘制代理
}

void Renderer::drawGrid() {
    glColor3f(1.0, 1.0, 1.0); // 设置绘制颜色为白色
    glBegin(GL_LINES);

    // 绘制水平线
    for (int i = 0; i <= 5; ++i) {
        float y = i * windowHeight / 5.0;
        glVertex2f(0, y);
        glVertex2f(windowWidth, y);
    }

    // 绘制垂直线
    for (int i = 0; i <= 5; ++i) {
        float x = i * windowWidth / 5.0;
        glVertex2f(x, 0);
        glVertex2f(x, windowHeight);
    }

    glEnd();
}