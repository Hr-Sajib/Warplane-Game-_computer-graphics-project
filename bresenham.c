#include <GLUT/glut.h>
#include <math.h>

// Function to plot the 8 symmetric points
void plotCirclePoints(int xc, int yc, int x, int y) {
    glBegin(GL_POINTS);
        glVertex2i(xc + x, yc + y);
        glVertex2i(xc - x, yc + y);
        glVertex2i(xc + x, yc - y);
        glVertex2i(xc - x, yc - y);
        glVertex2i(xc + y, yc + x);
        glVertex2i(xc - y, yc + x);
        glVertex2i(xc + y, yc - x);
        glVertex2i(xc - y, yc - x);
    glEnd();
}

// Bresenham's Circle Drawing Algorithm
void bresenhamCircle(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int p = 3 - 2 * r;

    plotCirclePoints(xc, yc, x, y);

    while (x < y) {
        x++;
        if (p < 0) {
            p += 4 * x + 6;
        } else {
            y--;
            p += 4 * (x - y) + 10;
        }
        plotCirclePoints(xc, yc, x, y);
    }
}

// Draw X and Y axes
void drawAxes() {
    glColor3f(0.0, 0.0, 1.0); // Blue axes
    glBegin(GL_LINES);
        glVertex2i(-500, 0); glVertex2i(500, 0);  // X-axis
        glVertex2i(0, -500); glVertex2i(0, 500);  // Y-axis
    glEnd();
}

// Display callback
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawAxes();

    glColor3f(1.0, 0.0, 0.0); // Red circle
    bresenhamCircle(0, 0, 200);

    glFlush();
}

// OpenGL Initialization
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glColor3f(1.0, 0.0, 0.0);        // Red drawing color
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
    glutCreateWindow("Bresenham's Circle Drawing Algorithm");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
