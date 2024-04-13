#include <GL/glut.h>

// ���崰�ڵĴ�С
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

// ��ʼ��OpenGLͼ����ͼ
void initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // ���ñ�����ɫΪ��ɫ
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);
}

// ����5x5�ĸ���ͼ
void drawGrid() {
    glColor3f(0.0, 1.0, 0.0); // ���û�����ɫΪ��ɫ
    glBegin(GL_LINES);

    // ����ˮƽ��
    for (int i = 0; i <= 5; ++i) {
        glVertex2f(0, i * WINDOW_HEIGHT / 5);
        glVertex2f(WINDOW_WIDTH, i * WINDOW_HEIGHT / 5);
    }

    // ���ƴ�ֱ��
    for (int i = 0; i <= 5; ++i) {
        glVertex2f(i * WINDOW_WIDTH / 5, 0);
        glVertex2f(i * WINDOW_WIDTH / 5, WINDOW_HEIGHT);
    }

    glEnd();
}

// ��ʾ�ص�����
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // �����ɫ������
    drawGrid(); // ���Ƹ���ͼ
    glFlush(); // ����ִ�л�������
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // ��ʼ��GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // ������ʾģʽ
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // ���ô��ڴ�С
    glutInitWindowPosition(100, 100); // ���ô���λ��
    glutCreateWindow("5x5 State Space with OpenGL"); // ��������

    initOpenGL(); // ��ʼ��OpenGLͼ����ͼ
    glutDisplayFunc(display); // ע����ʾ�ص�����
    glutMainLoop(); // ����GLUT�¼�����ѭ��

    return 0;
}
