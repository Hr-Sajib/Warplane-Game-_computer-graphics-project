#include <GL/freeglut.h>


// square
void drawSquare(float x, float y, float size) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + size, y);
    glVertex2f(x + size, y + size);
    glVertex2f(x, y + size);
    glEnd();
}

// triangle 
void drawTriangle(float x, float y, float base, float height) {
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y);
    glVertex2f(x + base, y);
    glVertex2f(x + base / 2, y + height);
    glEnd();
}

// rectangle
void drawRectangle(float x, float y, float width, float height) {
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);


    glColor3f(0.0f, 0.0f, 1.0f);
    drawSquare(-0.3f, -0.3f, 0.6f);



    glColor3f(1.0f, 0.0f, 0.0f);
    drawTriangle(-0.35f, 0.3f, 0.7f, 0.4f);


    glColor3f(0.5f, 0.25f, 0.0f);
    drawRectangle(-0.08f, -0.3f, 0.16f, 0.25f);


    glColor3f(1.0f, 1.0f, 0.0f);
    drawRectangle(-0.25f, 0.0f, 0.12f, 0.12f);
    drawRectangle(0.13f, 0.0f, 0.12f, 0.12f);

    glFlush();
}

// Initialization 
void init() {
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f); // Light gray background
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1, 1, -1, 1);
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("House Structure");

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
