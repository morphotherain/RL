#ifndef RENDERER_H
#define RENDERER_H

#include "Environment.h"
#include "Agent.h"

class Renderer {
private:
    Environment* environment;
    Agent* agent;
    int windowWidth = 1000;
    int windowHeight = 1000;
public:
    Renderer(Environment* env, Agent* agt) : environment(env), agent(agt), windowWidth(100*env->getGridSize()), windowHeight(100 * env->getGridSize()) {}
    void drawEnvironment(); // ���ƻ���
    void drawAgent(); // ���ƴ���
    void drawGrid(); // ��������
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    // ������Ⱦ��صķ���...
};

// ������������ָ��λ�û����ı�
void drawText(const char* text, float x, float y);


#endif
