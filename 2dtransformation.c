#include <GLUT/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

float triangle[3][2] = {
    {0.0, 0.5},
    {-0.5, -0.5},
    {0.5, -0.5}
};

float tx = 0, ty = 0;
float angle = 0;
float shearX = 0, shearY = 0;
int reflectX = 1, reflectY = 1;

void resetTransformations() {
    tx = ty = 0;
    angle = 0;
    shearX = shearY = 0;
    reflectX = reflectY = 1;
    printf("\n--- Transformations Reset ---\n");
}

void drawAxis() {
    glColor3f(0.5, 0.5, 0.5); // Grey axis
    glBegin(GL_LINES);
        glVertex2f(-1.0, 0.0); glVertex2f(1.0, 0.0);  // X-axis
        glVertex2f(0.0, -1.0); glVertex2f(0.0, 1.0);  // Y-axis
    glEnd();
}

void drawTriangle() {
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 1.0); // Light blue
    for (int i = 0; i < 3; i++) {
        float x = triangle[i][0];
        float y = triangle[i][1];

        // Apply shearing
        x = x + shearX * y;
        y = y + shearY * x;

        // Apply reflection
        x *= reflectX;
        y *= reflectY;

        // Apply rotation
        float rad = angle * 3.1416 / 180;
        float x_rot = x * cos(rad) - y * sin(rad);
        float y_rot = x * sin(rad) + y * cos(rad);

        // Apply translation
        x_rot += tx;
        y_rot += ty;

        glVertex2f(x_rot, y_rot);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxis();
    drawTriangle();
    glFlush();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': ty += 0.05; break;
        case 's': ty -= 0.05; break;
        case 'a': tx -= 0.05; break;
        case 'd': tx += 0.05; break;
        case 'r': angle += 10; break;
        case 'x': shearX += 0.1; break;
        case 'y': shearY += 0.1; break;
        case '1': reflectX *= -1; break;
        case '2': reflectY *= -1; break;
        case 'z': resetTransformations(); break;
        case 27: exit(0);  // ESC key
    }
    glutPostRedisplay();
}

void printInstructions() {
    printf("----- 2D Transformation Controls -----\n");
    printf("Translation:   W (up), A (left), S (down), D (right)\n");
    printf("Rotation:      R\n");
    printf("Shearing:      X (shear in X), Y (shear in Y)\n");
    printf("Reflection:    1 (reflect over X-axis), 2 (reflect over Y-axis)\n");
    printf("Reset:         Z\n");
    printf("Exit:          ESC\n");
    printf("--------------------------------------\n");
}

void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // white background
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-1, 1, -1, 1);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("2D Transformations");
    init();
    printInstructions();  // Show instructions when program starts
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
