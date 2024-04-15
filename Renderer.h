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
    void drawEnvironment(); // 绘制环境
    void drawAgent(); // 绘制代理
    void drawGrid(); // 绘制网格
    int getWindowWidth() const { return windowWidth; }
    int getWindowHeight() const { return windowHeight; }
    // 更多渲染相关的方法...
};

// 辅助函数：在指定位置绘制文本
void drawText(const char* text, float x, float y);


#endif
