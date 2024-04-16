#include "Renderer.h"
#include <GL/glut.h>


#include <cmath> // For cos and sin functions

// ��������������״ֵ̬ѡ����ɫ
void setColorBasedOnValue(float value, float max) {
    // ʾ����ʹ�ü򵥵�����ӳ�佫״ֵ̬ӳ�䵽��ɫ����ɫ�Ľ���
    float normalizedValue = std::min(std::max(value/ std::max(max, 10.0f), 0.0f), 1.0f); // ȷ��ֵ��0��1֮��
    glColor3f(normalizedValue, 0.0, 1.0 - normalizedValue);
}

// ������������ָ��λ�û����ı�
void drawText(const char* text, float x, float y) {
    glRasterPos2f(x, y); // �����ı���ʼ���Ƶ�λ��
    for (const char* c = text; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c); // ʹ�ù̶���ȵ���������ı�
    }
}


void drawPolicyIndicator(float x, float y, float cellWidth, float cellHeight, const std::map<ActionType, float>& actionProbabilities) {
    // �����������ӳ��
    std::map<ActionType, std::pair<float, float>> directions = {
        {ActionType::Up, {0, -1}},
        {ActionType::Down, {0, 1}},
        {ActionType::Left, {-1, 0}},
        {ActionType::Right, {1, 0}}
    };

    for (const auto& actionProbability : actionProbabilities) {
        auto direction = directions[actionProbability.first];
        float length = std::sqrt(actionProbability.second) * std::min(cellWidth, cellHeight) / 2; // ʹ�ø��ʵ�ƽ���������ų���

        // ���ݸ���������ɫ������ɫ���͸��ʣ�����ɫ���߸��ʣ�
        float colorIntensity = actionProbability.second; // ���������0��1֮��
        glColor3f(colorIntensity, 0.0, 1.0 - colorIntensity);

        // ���������ԭ�ز����������һ��Բ�ܱ�ʾ����
        if (actionProbability.first == ActionType::Stay) {
            float radius = length / 3.0f; // ʹ�ø�����ȷ��Բ�İ뾶
            glBegin(GL_LINE_LOOP); // �޸�ΪGL_LINE_LOOP���ƿ���Բ
            for (int i = 0; i < 360; i++) {
                float degInRad = i * (3.14159f / 180.0f);
                glVertex2f(cos(degInRad) * radius + x + cellWidth / 2, sin(degInRad) * radius + y + cellHeight / 2);
            }
            glEnd();
        }
        else {
            // ���򣬸��ݸ��ʻ����߶�
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
                glColor3f(1.0, 1.0, 0.8); // ����ɫ
                break;
            case CellType::Target:
                glColor3f(0.8, 1.0, 0.8); // ������ɫ
                break;
            }
            // �����������ͳߴ�
            float x = i * windowWidth / grid.size();
            float y = j * windowHeight / grid[i].size();
            float cellWidth = windowWidth / grid.size();
            float cellHeight = windowHeight / grid[i].size();
            // ���Ƹ���
            glBegin(GL_QUADS);
            glVertex2f(x, y);
            glVertex2f(x + cellWidth, y);
            glVertex2f(x + cellWidth, y + cellHeight);
            glVertex2f(x, y + cellHeight);
            glEnd();
            // ���Ƹ��ӱ߿�
            glColor3f(0.8, 0.8, 0.8); // ����ɫ
            glLineWidth(1);
            glBegin(GL_LINE_LOOP);
            glVertex2f(x, y);
            glVertex2f(x + cellWidth, y);
            glVertex2f(x + cellWidth, y + cellHeight);
            glVertex2f(x, y + cellHeight);
            glEnd();
        }
    }
    float maxStateValue = 0.0f;

    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            if (stateValues[i][j] > maxStateValue)
                maxStateValue = stateValues[i][j];
        }
    }
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            float x = i * windowWidth / grid.size();
            float y = j * windowHeight / grid[i].size();
            float cellWidth = windowWidth / grid.size();
            float cellHeight = windowHeight / grid[i].size();

            // ���û���״ֵ̬����ɫ
            setColorBasedOnValue(stateValues[i][j],maxStateValue);

            // ����״ֵ̬
            float stateValue = stateValues[i][j];
            char text[10];
            sprintf_s(text, "%.2f", stateValue); // ��״ֵ̬ת��Ϊ�ַ�����������λС��

            // �����ı����Ƶ���ʼλ�ã�ʹ��λ�ڸ�������
            float textWidth = glutBitmapLength(GLUT_BITMAP_8_BY_13, (const unsigned char*)text);
            float textX = x + (cellWidth - textWidth) / 2.0f - 20.0f;
            float textY = y + cellHeight / 2.0f - 10.0f; // GLUT_BITMAP_8_BY_13���ַ��߶ȴ�ԼΪ13����
            setColorBasedOnValue(stateValue, maxStateValue); // ���Ը�����Ҫ�����ı���ɫ
            drawText(text, textX, textY);

            //�ڸ�������
            float radius = 5.0f;
            glBegin(GL_POLYGON); 
            for (int i = 0; i < 360; i++) {
                float degInRad = i * (3.14159f / 180.0f);
                glVertex2f(cos(degInRad) * radius + x + cellWidth / 2, sin(degInRad) * radius + y + cellHeight / 2);
            }
            glEnd();

            // ��ȡ��ǰ���ӵĲ���
            auto actionProbabilities = agent->getStochasticPolicy({ i, j });

            // ���Ʋ���ָʾ��
            drawPolicyIndicator(x, y, cellWidth, cellHeight, actionProbabilities);
        }
    }
}


void Renderer::drawAgent() {
    // ʹ��OpenGL���ƴ���
}

void Renderer::drawGrid() {
    glColor3f(1.0, 1.0, 1.0); // ���û�����ɫΪ��ɫ
    glBegin(GL_LINES);

    // ����ˮƽ��
    for (int i = 0; i <= 5; ++i) {
        float y = i * windowHeight / 5.0;
        glVertex2f(0, y);
        glVertex2f(windowWidth, y);
    }

    // ���ƴ�ֱ��
    for (int i = 0; i <= 5; ++i) {
        float x = i * windowWidth / 5.0;
        glVertex2f(x, 0);
        glVertex2f(x, windowHeight);
    }

    glEnd();
}