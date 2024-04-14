#include <GL/glut.h>
#include "Environment.h"
#include "Agent.h"
#include "Renderer.h"

// 创建环境、代理和渲染器对象
Environment environment;
Agent agent;
Renderer renderer(&environment, &agent);

// 初始化OpenGL图形视图
void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // 设置背景颜色为黑色
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, renderer.getWindowWidth(), renderer.getWindowHeight(), 0.0);
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区
    renderer.drawEnvironment(); // 绘制环境
    renderer.drawAgent(); // 绘制代理
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
    glutMainLoop(); // 进入GLUT事件处理循环

    return 0;
}
