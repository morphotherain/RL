#include <GL/glut.h>
#include "Environment.h"
#include "Agent.h"
#include "Renderer.h"
#include <iostream>

// �����������������Ⱦ������
Environment environment;
Agent agent(&environment);
Renderer renderer(&environment, &agent);
float desiredFPS = 0.5;

// ��ʼ��OpenGLͼ����ͼ
void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // ���ñ�����ɫΪ��ɫ
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, renderer.getWindowWidth(), renderer.getWindowHeight(), 0.0);
}

void onTimer(int value) {
    agent.run(); // ִ��һ���㷨
    glutPostRedisplay(); // ��ǵ�ǰ������Ҫ�����»���
    glutTimerFunc(1000 / desiredFPS, onTimer, 0); // �ٴ�ע�ᶨʱ��
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT); // �����ɫ������

    renderer.drawEnvironment(); // ���ƻ���
    renderer.drawAgent(); // ���ƴ���

    // ���Ƶ�ǰ�㷨������
    char stepText[50];
    sprintf_s(stepText, "Step: %d", agent.getStepCount()); // ��ʽ���������������ַ���
    glColor3f(1.0f, 0.0f, 0.0f); // �����ı���ɫ
    drawText(stepText, 10, 20); // �ڴ������Ͻǻ����ı�
    std::cout << stepText << std::endl;

    glFlush(); // ����ִ�л�������
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // ��ʼ��GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // ������ʾģʽ
    glutInitWindowSize(renderer.getWindowWidth(), renderer.getWindowHeight()); // ���ô��ڴ�С
    glutInitWindowPosition(100, 100); // ���ô���λ��
    glutCreateWindow("Reinforcement Learning Environment"); // ��������

    initOpenGL(); // ��ʼ��OpenGLͼ����ͼ
    glutDisplayFunc(display); // ע����ʾ�ص�����
    glutTimerFunc(1000 / desiredFPS, onTimer, 0); // desiredFPS������Ҫ��֡��
    glutMainLoop(); // ����GLUT�¼�����ѭ��

    return 0;
}
