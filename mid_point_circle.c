#include <GLUT/glut.h>  // Include the GLUT library for OpenGL graphics functions
#include <math.h>        // Include the math library for mathematical operations

// Function to plot all 8 symmetric points of a circle
void drawCirclePoints(int xc, int yc, int x, int y) {
    glBegin(GL_POINTS);  // Start drawing points
        glVertex2i(xc + x, yc + y); // Plot the point in the first quadrant
        glVertex2i(xc - x, yc + y); // Plot the point in the second quadrant
        glVertex2i(xc + x, yc - y); // Plot the point in the third quadrant
        glVertex2i(xc - x, yc - y); // Plot the point in the fourth quadrant
        glVertex2i(xc + y, yc + x); // Plot the point in the fifth quadrant (symmetry)
        glVertex2i(xc - y, yc + x); // Plot the point in the sixth quadrant (symmetry)
        glVertex2i(xc + y, yc - x); // Plot the point in the seventh quadrant (symmetry)
        glVertex2i(xc - y, yc - x); // Plot the point in the eighth quadrant (symmetry)
    glEnd();  // End drawing points
}

// Midpoint Circle Drawing Algorithm
void midpointCircle(int xc, int yc, int r) {
    int x = 0, y = r;           // Initialize the starting point (x=0, y=r)
    int p = 1 - r;               // Initial decision parameter
    drawCirclePoints(xc, yc, x, y); // Plot the initial symmetric points

    while (x < y) {  // Continue until the x-coordinate exceeds the y-coordinate
        x++;          // Increment x (move horizontally)
        if (p < 0) {  // If the decision parameter is less than 0
            p += 2 * x + 1; // Choose the point to the right (east)
        } else {       // If the decision parameter is greater than or equal to 0
            y--;      // Move diagonally (north-east)
            p += 2 * (x - y) + 1;  // Adjust decision parameter accordingly
        }
        drawCirclePoints(xc, yc, x, y); // Plot the symmetric points after each iteration
    }
}

// Function to draw the X and Y axes
void drawAxes() {
    glColor3f(0.0, 0.0, 1.0); // Set color to blue for axes
    glBegin(GL_LINES);  // Start drawing lines
        glVertex2i(-500, 0); glVertex2i(500, 0);  // Draw the X-axis from -500 to 500
        glVertex2i(0, -500); glVertex2i(0, 500);  // Draw the Y-axis from -500 to 500
    glEnd();  // End drawing lines
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the window
    drawAxes();                   // Draw the X and Y axes

    glColor3f(1.0, 0.0, 0.0); // Set color to red for the circle
    midpointCircle(0, 0, 100); // Draw a circle with center (0, 0) and radius 100

    glFlush();  // Ensure all drawing commands are executed
}

// OpenGL Initialization function
void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0); // Set background color to white
    glColor3f(1.0, 0.0, 0.0);         // Set initial drawing color to red
    glPointSize(2.0);                 // Set point size for drawing
    glMatrixMode(GL_PROJECTION);      // Set the projection matrix mode
    glLoadIdentity();                 // Load identity matrix
    gluOrtho2D(-500, 500, -500, 500); // Set orthographic projection with specified window size
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);  // Initialize GLUT
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); // Set display mode (single-buffered and RGB color model)
    glutInitWindowSize(600, 600);  // Set window size to 600x600 pixels
    glutInitWindowPosition(100, 100); // Set window position on screen
    glutCreateWindow("Mid-Point Circle Drawing Algorithm with Axes"); // Create window with title
    init();  // Call initialization function
    glutDisplayFunc(display); // Register the display callback function
    glutMainLoop(); // Enter GLUT main loop to display the window
    return 0;  // Return 0 to indicate successful execution
}
