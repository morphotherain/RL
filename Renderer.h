#ifndef RENDERER_H
#define RENDERER_H

#include "Environment.h"
#include "Agent.h"

class Renderer {
private:
    Environment* environment;
    Agent* agent;
    int windowWidth = 500;
    int windowHeight = 500;
public:
    Renderer(Environment* env, Agent* agt) : environment(env), agent(agt) {}
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
