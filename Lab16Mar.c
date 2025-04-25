#include <GLUT/glut.h>  // Use GLUT for macOS
#include <math.h>

// Function to plot points in all octants
void drawCirclePoints(int xc, int yc, int x, int y) {
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

// Function to draw axes
void drawAxes() {
    glBegin(GL_LINES);
    // X-axis (from left to right)
    glVertex2i(0, 250);
    glVertex2i(500, 250);
    
    // Y-axis (from bottom to top)
    glVertex2i(250, 0);
    glVertex2i(250, 500);
    glEnd();
}

// Bresenham's circle drawing algorithm
void circleBres(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 1 - r;
    drawCirclePoints(xc, yc, x, y);

    while (y >= x) {
        x++;
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
        drawCirclePoints(xc, yc, x, y);
    }
}

// Display callback for OpenGL
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Set color to black for axes
    glColor3f(0.0f, 0.0f, 0.0f);
    drawAxes(); // Draw the axes

    // Set color to red for the circle
    glColor3f(1.0f, 0.0f, 0.0f);
    circleBres(250, 250, 100);   // Draw circle at (250, 250) with radius 100
    
    glFlush();
}

// Initialize OpenGL
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // White background
    glColor3f(1.0, 0.0, 0.0);         // Drawing color red
    glPointSize(2.0);                 // Point size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 500, 0, 500);       // 2D orthographic projection
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Bresenham's Circle Drawing with Axes (macOS)");
    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
