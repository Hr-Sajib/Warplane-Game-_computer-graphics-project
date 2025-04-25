#include <GL/freeglut.h>
#include <math.h>

#define PI 3.14159265358979323846

// square (Green)
void drawSquare(float size) {
    glColor3f(0.0, 1.0, 0.0); // Green
    glBegin(GL_QUADS);
    glVertex2f(-size / 2, -size / 2);
    glVertex2f(size / 2, -size / 2);
    glVertex2f(size / 2, size / 2);
    glVertex2f(-size / 2, size / 2);
    glEnd();
}

// triangle (Blue)
void drawTriangle(float size) {
    glColor3f(0.0, 0.0, 1.0); // Blue
    glBegin(GL_TRIANGLES);
    glVertex2f(0.0f, size / 2);
    glVertex2f(-size / 2, -size / 2);
    glVertex2f(size / 2, -size / 2);
    glEnd();
}

// circle (Red)
void drawCircle(float radius, int segments) {
    glColor3f(1.0, 0.0, 0.0); // Red
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(0.0f, 0.0f);
    for (int i = 0; i <= segments; i++) {
        float angle = 2.0f * PI * i / segments;
        glVertex2f(cos(angle) * radius, sin(angle) * radius);
    }
    glEnd();
}

// pentagon (Yellow)
void drawPentagon(float size) {
    glColor3f(1.0, 1.0, 0.0); // Yellow
    glBegin(GL_POLYGON);
    for (int i = 0; i < 5; i++) {
        float angle = 2.0f * PI * i / 5;
        glVertex2f(cos(angle) * size, sin(angle) * size);
    }
    glEnd();
}

// hexagon (Magenta)
void drawHexagon(float size) {
    glColor3f(1.0, 0.0, 1.0); // Magenta
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; i++) {
        float angle = 2.0f * PI * i / 6;
        glVertex2f(cos(angle) * size, sin(angle) * size);
    }
    glEnd();
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Calling the shape
    drawTriangle(0.5f);  

    glutSwapBuffers();
}

// Main function (simplified)
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("2D Shapes");

    gluOrtho2D(-1, 1, -1, 1);  // Set coordinate system

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
