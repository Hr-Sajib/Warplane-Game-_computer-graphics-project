#include <GLUT/glut.h>
#include <stdlib.h>
#include <math.h>

void drawPixel(int x, int y) {
    glBegin(GL_POINTS);
        glVertex2i(x, y);
    glEnd();
}

void midPointLine(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int x = x0;
    int y = y0;
    int p = 2 * dy - dx;

    int xEnd = x1;
    int stepX = (x1 > x0) ? 1 : -1;
    int stepY = (y1 > y0) ? 1 : -1;

    drawPixel(x, y);

    for (int i = 0; i < dx; i++) {
        x += stepX;
        if (p < 0) {
            p += 2 * dy;
        } else {
            y += stepY;
            p += 2 * (dy - dx);
        }
        drawPixel(x, y);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1.0, 0.0, 0.0); // Red color

    // Draw axes
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
        glVertex2i(-500, 0); glVertex2i(500, 0);
        glVertex2i(0, -500); glVertex2i(0, 500);
    glEnd();

    glColor3f(1.0, 0.0, 0.0);
    midPointLine(-200, -100, 200, 100); // Example line
    glFlush();
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glColor3f(1.0, 0.0, 0.0);         // Red drawing color
    glPointSize(2.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-500, 500, -500, 500);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Mid-Point Line Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
