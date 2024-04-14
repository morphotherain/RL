#include <GL/glut.h>
#include "Environment.h"
#include "Agent.h"
#include "Renderer.h"

// �����������������Ⱦ������
Environment environment;
Agent agent;
Renderer renderer(&environment, &agent);

// ��ʼ��OpenGLͼ����ͼ
void initOpenGL() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // ���ñ�����ɫΪ��ɫ
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, renderer.getWindowWidth(), renderer.getWindowHeight(), 0.0);
}

// ��ʾ�ص�����
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // �����ɫ������
    renderer.drawEnvironment(); // ���ƻ���
    renderer.drawAgent(); // ���ƴ���
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
    glutMainLoop(); // ����GLUT�¼�����ѭ��

    return 0;
}
