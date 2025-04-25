#include <GLUT/glut.h>  // Include the GLUT library for OpenGL graphics functions
#include <stdlib.h>     // Include stdlib.h for the exit() function

// Global variables for camera control
float angle = 0.0f;  // Camera rotation angle
float camX = 0.0f, camY = 5.0f, camZ = 20.0f;  // Camera position

// Function to draw a 3D cube (rectangular prism)
void drawCube(float x, float y, float z, float width, float height, float depth) {
    glPushMatrix();
    glTranslatef(x, y, z);  // Position the cube
    glScalef(width, height, depth);  // Scale to desired dimensions
    glutSolidCube(1.0);  // Draw a unit cube scaled to size
    glPopMatrix();
}

// Function to draw a flat plane (for the field)
void drawField() {
    glColor3f(0.0, 0.8, 0.0);  // Green color for the field
    glBegin(GL_QUADS);
        glVertex3f(-50.0, 0.0, -50.0);  // Bottom-left
        glVertex3f(50.0, 0.0, -50.0);   // Bottom-right
        glVertex3f(50.0, 0.0, 50.0);    // Top-right
        glVertex3f(-50.0, 0.0, 50.0);   // Top-left
    glEnd();
}

// Function to draw a simplified tree (as a cone for the foliage and a cube for the trunk)
void drawTree(float x, float z) {
    // Trunk
    glColor3f(0.55, 0.27, 0.07);  // Brown color for the trunk
    drawCube(x, 1.0, z, 0.5, 2.0, 0.5);  // Small cube for the trunk

    // Foliage (using a cone)
    glColor3f(0.0, 0.5, 0.0);  // Dark green for foliage
    glPushMatrix();
    glTranslatef(x, 3.0, z);  // Position the cone above the trunk
    glutSolidCone(1.5, 3.0, 10, 10);  // Draw a cone for the foliage
    glPopMatrix();
}

// Function to draw the building (simplified as a combination of cubes)
void drawBuilding() {
    // Main building structure (simplified as a large cube)
    glColor3f(0.9, 0.9, 0.9);  // Light gray for the building
    drawCube(0.0, 5.0, -20.0, 20.0, 10.0, 5.0);  // Main wide base

    // Left tower (taller section on the left)
    glColor3f(0.8, 0.8, 0.8);  // Slightly darker gray
    drawCube(-7.0, 12.0, -20.0, 6.0, 14.0, 5.0);  // Taller section on the left

    // Right tower (taller section on the right)
    glColor3f(0.8, 0.8, 0.8);
    drawCube(7.0, 12.0, -20.0, 6.0, 14.0, 5.0);  // Taller section on the right

    // Central curved section (approximated as a smaller cube with a different color)
    glColor3f(0.7, 0.5, 0.3);  // Brownish color for the central curved area
    drawCube(0.0, 3.0, -17.0, 10.0, 3.0, 2.0);  // Smaller cube for the curved entrance
}

// Display callback function
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Clear the color and depth buffers
    glLoadIdentity();

    // Set up the camera
    gluLookAt(camX, camY, camZ,  // Camera position
              0.0, 5.0, -20.0,   // Look-at point (center of the building)
              0.0, 1.0, 0.0);    // Up vector

    // Draw the sky (as a background gradient)
    glBegin(GL_QUADS);
        glColor3f(0.0, 0.5, 1.0);  // Light blue at the top
        glVertex3f(-100.0, 50.0, -100.0);
        glVertex3f(100.0, 50.0, -100.0);
        glColor3f(0.8, 0.9, 1.0);  // Lighter blue at the bottom
        glVertex3f(100.0, 0.0, -100.0);
        glVertex3f(-100.0, 0.0, -100.0);
    glEnd();

    // Draw the field
    drawField();

    // Draw the building
    drawBuilding();

    // Draw some trees
    drawTree(-20.0, 10.0);  // Tree on the left
    drawTree(-15.0, 5.0);   // Another tree on the left
    drawTree(20.0, 0.0);    // Tree on the right

    glutSwapBuffers();  // Swap buffers for smooth animation
}

// Function to handle keyboard input for camera control
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 27:  // ESC key to exit
            exit(0);
            break;
        case 'w':  // Move camera forward
            camZ -= 1.0;
            break;
        case 's':  // Move camera backward
            camZ += 1.0;
            break;
        case 'a':  // Move camera left
            camX -= 1.0;
            break;
        case 'd':  // Move camera right
            camX += 1.0;
            break;
        case 'q':  // Move camera up
            camY += 1.0;
            break;
        case 'e':  // Move camera down
            camY -= 1.0;
            break;
    }
    glutPostRedisplay();  // Request a redraw
}

// OpenGL Initialization function
void init() {
    glClearColor(0.8, 0.9, 1.0, 1.0);  // Set background color to light blue (sky)
    glEnable(GL_DEPTH_TEST);  // Enable depth testing for 3D rendering
    glMatrixMode(GL_PROJECTION);  // Set the projection matrix mode
    glLoadIdentity();
    gluPerspective(45.0, 1.5, 1.0, 100.0);  // Set perspective projection (FOV, aspect ratio, near, far)
    glMatrixMode(GL_MODELVIEW);  // Switch to modelview matrix
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);  // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Set display mode (double-buffered, RGB, depth)
    glutInitWindowSize(900, 600);  // Set window size to 900x600 pixels
    glutInitWindowPosition(100, 100);  // Set window position on screen
    glutCreateWindow("Daffodil International University Building - 3D View");  // Create window with title
    init();  // Call initialization function
    glutDisplayFunc(display);  // Register the display callback function
    glutKeyboardFunc(keyboard);  // Register the keyboard callback function
    glutMainLoop();  // Enter GLUT main loop to display the window
    return 0;  // Return 0 to indicate successful execution
}