#include <GL/glut.h>
#include "Environment.h"
#include "Agent.h"
#include "Renderer.h"
#include <iostream>

// 创建环境、代理和渲染器对象
Environment environment;
Agent agent(&environment);
Renderer renderer(&environment, &agent);
float desiredFPS = 0.5;

// 初始化OpenGL图形视图
void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // 设置背景颜色为黑色
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, renderer.getWindowWidth(), renderer.getWindowHeight(), 0.0);
}

void onTimer(int value) {
    agent.run(); // 执行一步算法
    glutPostRedisplay(); // 标记当前窗口需要被重新绘制
    glutTimerFunc(1000 / desiredFPS, onTimer, 0); // 再次注册定时器
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区

    renderer.drawEnvironment(); // 绘制环境
    renderer.drawAgent(); // 绘制代理

    // 绘制当前算法步骤数
    char stepText[50];
    sprintf_s(stepText, "Step: %d", agent.getStepCount()); // 格式化包含步骤数的字符串
    glColor3f(1.0f, 0.0f, 0.0f); // 设置文本颜色
    drawText(stepText, 10, 20); // 在窗口左上角绘制文本
    std::cout << stepText << std::endl;

    glFlush(); // 立即执行绘制命令
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // 初始化GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // 设置显示模式
    glutInitWindowSize(renderer.getWindowWidth(), renderer.getWindowHeight()); // 设置窗口大小
    glutInitWindowPosition(100, 100); // 设置窗口位置
    glutCreateWindow("Reinforcement Learning Environment"); // 创建窗口

    initOpenGL(); // 初始化OpenGL图形视图
    glutDisplayFunc(display); // 注册显示回调函数
    glutTimerFunc(1000 / desiredFPS, onTimer, 0); // desiredFPS是你想要的帧率
    glutMainLoop(); // 进入GLUT事件处理循环

    return 0;
}
