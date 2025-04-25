#include <GL/freeglut.h>
#include <math.h>

#define PI 3.14159265358979323846

// Rotation angle for the 3D objects
float angle = 0.0;

// Function to draw a sphere
void drawSphere(float radius, int slices, int stacks) {
    glutWireSphere(radius, slices, stacks); // Draw a wireframe sphere
}

// Function to draw a cube
void drawCube(float size) {
    glutWireCube(size); // Draw a wireframe cube
}

// Function to draw a pyramid
void drawPyramid(float base, float height) {
    glBegin(GL_TRIANGLES);

    // Front face
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(-base, -base, -base);
    glVertex3f(base, -base, -base);

    // Right face
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(base, -base, -base);
    glVertex3f(base, -base, base);

    // Back face
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(base, -base, base);
    glVertex3f(-base, -base, base);

    // Left face
    glVertex3f(0.0f, height, 0.0f);
    glVertex3f(-base, -base, base);
    glVertex3f(-base, -base, -base);

    glEnd();

    glBegin(GL_QUADS);

    // Base face
    glVertex3f(-base, -base, -base);
    glVertex3f(base, -base, -base);
    glVertex3f(base, -base, base);
    glVertex3f(-base, -base, base);

    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear the screen

    glLoadIdentity(); // Load identity matrix for transformations
    glTranslatef(0.0f, 0.0f, -5.0f); // Move the scene back along Z-axis

    // Rotate objects based on the angle
    glRotatef(angle, 1.0f, 1.0f, 0.0f); // Rotate around X and Y axis

    // Draw the cube (Yellow)
    glColor3f(1.0, 1.0, 0.0); // Yellow
    drawCube(1.0f); // Cube size 1.0

    // Draw the sphere (Red)
    glPushMatrix(); // Save the current matrix
    glTranslatef(2.0f, 0.0f, 0.0f); // Move the sphere along X-axis
    glColor3f(1.0, 0.0, 0.0); // Red
    drawSphere(0.5f, 20, 20); // Sphere with radius 0.5
    glPopMatrix(); // Restore the matrix

    // Draw the pyramid (Blue)
    glPushMatrix(); // Save the current matrix
    glTranslatef(-2.0f, 0.0f, 0.0f); // Move the pyramid along X-axis
    glColor3f(0.0, 0.0, 1.0); // Blue
    drawPyramid(1.0f, 1.5f); // Pyramid base size 1.0 and height 1.5
    glPopMatrix(); // Restore the matrix

    glutSwapBuffers(); // Swap the buffers for smooth rendering
}

void init() {
    glClearColor(0.0, 0.0, 0.0, 1.0); // Set background color to black
    glEnable(GL_DEPTH_TEST); // Enable depth testing for 3D rendering
    glMatrixMode(GL_PROJECTION); // Set projection matrix
    glLoadIdentity(); // Load identity matrix
    gluPerspective(45.0, 1.0, 1.0, 200.0); // Set perspective projection
    glMatrixMode(GL_MODELVIEW); // Set model-view matrix
}

void update(int value) {
    angle += 2.0f; // Increment rotation angle
    if (angle > 360) {
        angle -= 360; // Keep the angle in range [0, 360]
    }
    glutPostRedisplay(); // Redraw the screen
    glutTimerFunc(16, update, 0); // Call the update function every 16ms
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Enable double buffering and depth buffer
    glutInitWindowSize(500, 500); // Set window size
    glutCreateWindow("3D Complex Structure"); // Create the window with title

    init(); // Initialize OpenGL settings

    glutDisplayFunc(display); // Set display function callback
    glutTimerFunc(25, update, 0); // Set timer function for continuous update

    glutMainLoop(); // Enter the GLUT main loop

    return 0;
}





