#include <GL/glut.h>

// 定义窗口的大小
const int WINDOW_WIDTH = 500;
const int WINDOW_HEIGHT = 500;

// 初始化OpenGL图形视图
void initOpenGL() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // 设置背景颜色为黑色
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, WINDOW_WIDTH, WINDOW_HEIGHT, 0.0);
}

// 绘制5x5的格子图
void drawGrid() {
    glColor3f(0.0, 1.0, 0.0); // 设置绘制颜色为白色
    glBegin(GL_LINES);

    // 绘制水平线
    for (int i = 0; i <= 5; ++i) {
        glVertex2f(0, i * WINDOW_HEIGHT / 5);
        glVertex2f(WINDOW_WIDTH, i * WINDOW_HEIGHT / 5);
    }

    // 绘制垂直线
    for (int i = 0; i <= 5; ++i) {
        glVertex2f(i * WINDOW_WIDTH / 5, 0);
        glVertex2f(i * WINDOW_WIDTH / 5, WINDOW_HEIGHT);
    }

    glEnd();
}

// 显示回调函数
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // 清除颜色缓冲区
    drawGrid(); // 绘制格子图
    glFlush(); // 立即执行绘制命令
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // 初始化GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // 设置显示模式
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // 设置窗口大小
    glutInitWindowPosition(100, 100); // 设置窗口位置
    glutCreateWindow("5x5 State Space with OpenGL"); // 创建窗口

    initOpenGL(); // 初始化OpenGL图形视图
    glutDisplayFunc(display); // 注册显示回调函数
    glutMainLoop(); // 进入GLUT事件处理循环

    return 0;
}
