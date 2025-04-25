#include <GLUT/glut.h> // GLUT and OpenGL utilities for window and rendering
#include <cmath>       // For cos and sin in circle drawing
#include <vector>      // For managing projectiles and enemies
#include <cstdlib>     // For rand()
#include <ctime>       // For srand(time(nullptr))

// Structure for a projectile
struct Projectile {
    float x, y;     // Position
    bool active;    // Whether the projectile is still on screen
    Projectile(float startX, float startY) : x(startX), y(startY), active(true) {}
};

// Structure for a missile
struct Missile {
    float x, y;     // Position
    bool active;    // Whether the missile is still on screen
    Missile(float startX, float startY) : x(startX), y(startY), active(true) {}
};

// Structure for an enemy plane
struct EnemyPlane {
    float x, y;     // Position
    bool active;    // Whether the enemy is still on screen
    EnemyPlane(float startX, float startY) : x(startX), y(startY), active(true) {}
};

// Structure for a fireball
struct Fireball {
    float x, y;     // Position (center of explosion)
    int duration;   // Frames remaining to display
    Fireball(float posX, float posY) : x(posX), y(posY), duration(5) {} // 5 frames (~83ms at 60 FPS)
};

// Global variables
std::vector<Projectile> projectiles;
std::vector<Missile> missiles;
std::vector<EnemyPlane> enemies;
std::vector<Fireball> fireballs; // Vector for fireballs
const float PROJECTILE_SPEED = 10.0f; // Pixels per frame (used for both projectiles and missiles)
const float PROJECTILE_RADIUS = 6.0f; // Size of projectile
const float MISSILE_EXPLOSION_RADIUS = 200.0f; // Radius for missile explosion effect
const float FIREBALL_RADIUS = 75.0f; // Size of fireball
const int SHOOT_INTERVAL = 10;        // Frames between shots
const float ENEMY_SPEED = 3.0f;       // Pixels per frame for enemy movement
const int ENEMY_SPAWN_INTERVAL = 60;  // Frames between enemy spawns (~1s at 60 FPS)
int frameCount = 0;                   // To control shooting and enemy spawning
bool isFiring = false;                // Controls whether plane is firing projectiles
bool isFiringMissile = false;         // Controls whether plane is firing a missile
float planeY = 550.0f;                // Player plane's y-coordinate (initially centered)
bool moveUp = false;                  // Track up arrow state
bool moveDown = false;                // Track down arrow state
const float PLANE_SPEED = 5.0f;       // Pixels per frame for player plane movement
const float PLANE_Y_MIN = 50.0f;      // Top boundary (y=0 is bottom, so min is higher up)
const float PLANE_Y_MAX = 950.0f;     // Bottom boundary
int kills = 0;                        // Track number of enemy planes destroyed
int bullets = 100;                    // Track number of bullets (initially 100)
int missilesCount = 5;                // Track number of missiles (initially 5)
int lives = 3;                        // Track player lives (initially 3)
bool showGameOver = false;            // Flag to show game over screen
int gameOverTimer = 0;                // Frames to display game over (120 frames ~2s at 60 FPS)
int finalKills = 0;                   // Store kills for game over display
bool blinkActive = false;             // Whether the player plane is blinking
int blinkCount = 0;                   // Frame counter for current blink phase
int blinkPhase = 0;                   // Blink phase: 0=off, 1=on, 2=off, 3=on

// Draw a filled circle at (cx, cy) with given radius, with color and opacity based on type
void drawCircle(float cx, float cy, float radius, int type)
{
    const int segments = 50; // Number of triangles to approximate circle
    if (type == 0) // Projectile
        glColor4f(1.0f, 0.5f, 0.5f, 1.0f); // Light red, fully opaque
    else if (type == 1) // Cloud
        glColor4f(1.0f, 1.0f, 1.0f, 0.6f); // White, 60% opacity
    else if (type == 2) // Fireball
        glColor4f(0.910f, 0.282f, 0.090f, 0.8f); // RGB (232, 72, 23), 80% opacity
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(cx, cy); // Center of circle
    for (int i = 0; i <= segments; ++i)
    {
        float angle = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = cx + radius * cosf(angle);
        float y = cy + radius * sinf(angle);
        glVertex2f(x, y);
    }
    glEnd();
}

// Draw a missile at (cx, cy), shaped like a pointy pen
void drawMissile(float cx, float cy)
{
    // Missile dimensions (pen-like shape)
    const float bodyLength = 40.0f; // Length of the pen's body (shaft)
    const float bodyHeight = 4.0f;  // Height of the pen's body (narrow)
    const float tipLength = 10.0f;  // Length of the sharp tip
    const float tipHeight = 6.0f;   // Height of the tip at its base

    // Body: Narrow rectangle (pen shaft)
    glColor3f(0.7f, 0.7f, 0.7f); // Light gray for the pen body
    glBegin(GL_QUADS);
    glVertex2f(cx - bodyLength, cy - bodyHeight / 2.0f); // Rear bottom
    glVertex2f(cx, cy - bodyHeight / 2.0f);             // Front bottom
    glVertex2f(cx, cy + bodyHeight / 2.0f);             // Front top
    glVertex2f(cx - bodyLength, cy + bodyHeight / 2.0f); // Rear top
    glEnd();

    // Tip: Sharp triangle (pen point)
    glColor3f(0.9f, 0.9f, 0.9f); // Lighter gray for the tip
    glBegin(GL_TRIANGLES);
    glVertex2f(cx, cy);                        // Sharp tip
    glVertex2f(cx + tipLength, cy - tipHeight / 2.0f); // Bottom base
    glVertex2f(cx + tipLength, cy + tipHeight / 2.0f); // Top base
    glEnd();

    // Outline: Black lines for definition
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    // Body outline
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx - bodyLength, cy - bodyHeight / 2.0f);
    glVertex2f(cx, cy - bodyHeight / 2.0f);
    glVertex2f(cx, cy + bodyHeight / 2.0f);
    glVertex2f(cx - bodyLength, cy + bodyHeight / 2.0f);
    glEnd();
    // Tip outline
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx, cy);
    glVertex2f(cx + tipLength, cy - tipHeight / 2.0f);
    glVertex2f(cx + tipLength, cy + tipHeight / 2.0f);
    glEnd();
}

// Draw a war plane at (cx, cy), with scale and rightFacing option
void drawWarPlane(float cx, float cy, float scale, bool rightFacing)
{
    // Adjust coordinates based on scale and orientation
    float xSign = rightFacing ? -1.0f : 1.0f; // Flip x-coordinates: left-facing (xSign=1.0f), right-facing (xSign=-1.0f)

    // Body: Trapezoid shape
    if (rightFacing)
        glColor3f(1.0f, 1.0f, 1.0f); // White for enemy plane body
    else
        glColor3f(0.4f, 0.45f, 0.5f); // Light gray for player plane body
    glBegin(GL_QUADS);
    glVertex2f(cx + xSign * (-40.0f * scale), cy - 10.0f * scale); // Nose
    glVertex2f(cx, cy - 25.0f * scale);                           // Bottom mid
    glVertex2f(cx, cy + 25.0f * scale);                           // Top mid
    glVertex2f(cx + xSign * (-40.0f * scale), cy + 10.0f * scale); // Nose (top)
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(cx, cy - 25.0f * scale);                           // Bottom mid
    glVertex2f(cx + xSign * (40.0f * scale), cy - 15.0f * scale); // Bottom rear
    glVertex2f(cx + xSign * (40.0f * scale), cy + 15.0f * scale); // Top rear
    glVertex2f(cx, cy + 25.0f * scale);                           // Top mid
    glEnd();

    // Wings: Swept-back trapezoids
    glBegin(GL_QUADS);
    // Bottom wing
    glVertex2f(cx, cy - 25.0f * scale);                           // Root at body
    glVertex2f(cx + xSign * (10.0f * scale), cy - 50.0f * scale); // Leading edge
    glVertex2f(cx + xSign * (30.0f * scale), cy - 40.0f * scale); // Trailing edge
    glVertex2f(cx + xSign * (20.0f * scale), cy - 15.0f * scale); // Wingtip at body
    // Top wing
    glVertex2f(cx, cy + 25.0f * scale);                           // Root at body
    glVertex2f(cx + xSign * (10.0f * scale), cy + 50.0f * scale); // Leading edge
    glVertex2f(cx + xSign * (30.0f * scale), cy + 40.0f * scale); // Trailing edge
    glVertex2f(cx + xSign * (20.0f * scale), cy + 15.0f * scale); // Wingtip at body
    glEnd();

    // Tail Fins: Canted trapezoids
    glBegin(GL_QUADS);
    // Bottom fin
    glVertex2f(cx + xSign * (40.0f * scale), cy - 15.0f * scale); // Root
    glVertex2f(cx + xSign * (50.0f * scale), cy - 25.0f * scale); // Outer edge
    glVertex2f(cx + xSign * (60.0f * scale), cy - 20.0f * scale); // Tip
    glVertex2f(cx + xSign * (50.0f * scale), cy - 10.0f * scale); // Inner edge
    // Top fin
    glVertex2f(cx + xSign * (40.0f * scale), cy + 15.0f * scale); // Root
    glVertex2f(cx + xSign * (50.0f * scale), cy + 25.0f * scale); // Outer edge
    glVertex2f(cx + xSign * (60.0f * scale), cy + 20.0f * scale); // Tip
    glVertex2f(cx + xSign * (50.0f * scale), cy + 10.0f * scale); // Inner edge
    glEnd();

    // Engines/Nozzles: Small rectangles at rear
    if (rightFacing)
        glColor3f(0.8f, 0.8f, 0.8f); // Light gray for enemy plane engines
    else
        glColor3f(0.3f, 0.35f, 0.4f); // Darker gray for player plane engines
    glBegin(GL_QUADS);
    // Bottom engine
    glVertex2f(cx + xSign * (40.0f * scale), cy - 10.0f * scale);
    glVertex2f(cx + xSign * (40.0f * scale), cy - 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy - 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy - 10.0f * scale);
    // Top engine
    glVertex2f(cx + xSign * (40.0f * scale), cy + 10.0f * scale);
    glVertex2f(cx + xSign * (40.0f * scale), cy + 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy + 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy + 10.0f * scale);
    glEnd();

    // Blue star outline for enemy planes (right-facing)
    if (rightFacing)
    {
        glColor3f(0.0f, 0.0f, 1.0f); // Blue
        float outerRadius = 20.0f * scale; // Larger outer points of star
        float innerRadius = 8.0f * scale;  // Larger inner valleys of star
        glLineWidth(2.0f); // Bolder outline
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 12; ++i)
        {
            float angle = 3.1415926f / 2.0f + (i * 2.0f * 3.1415926f / 12.0f); // Start at 90° (up), 30° increments
            float radius = (i % 2 == 0) ? outerRadius : innerRadius;
            float x = cx + radius * cosf(angle);
            float y = cy + radius * sinf(angle);
            glVertex2f(x, y);
        }
        glEnd();
        glLineWidth(1.0f); // Reset line width
    }
    // Add "F22" text for player plane (left-facing)
    else
    {
        // Position the text at the center of the plane's body
        float textX = cx - 10.0f * scale; // Adjust left to roughly center "F22" (approx 20px wide at scale=1.0)
        float textY = cy + 5.0f * scale;  // Slightly above center for better visibility
        glColor3f(1.0f, 1.0f, 1.0f); // White text
        glRasterPos2f(textX, textY);
        const char* text = "F22";
        while (*text)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text++);
        }
    }

    // Outline: Black lines for definition
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    // Body outline
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx + xSign * (-40.0f * scale), cy - 10.0f * scale); // Nose
    glVertex2f(cx, cy - 25.0f * scale);                           // Bottom mid
    glVertex2f(cx + xSign * (40.0f * scale), cy - 15.0f * scale); // Bottom rear
    glVertex2f(cx + xSign * (40.0f * scale), cy + 15.0f * scale); // Top rear
    glVertex2f(cx, cy + 25.0f * scale);                           // Top mid
    glVertex2f(cx + xSign * (-40.0f * scale), cy + 10.0f * scale); // Nose (top)
    glEnd();
    // Bottom wing outline
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx, cy - 25.0f * scale);
    glVertex2f(cx + xSign * (10.0f * scale), cy - 50.0f * scale);
    glVertex2f(cx + xSign * (30.0f * scale), cy - 40.0f * scale);
    glVertex2f(cx + xSign * (20.0f * scale), cy - 15.0f * scale);
    glEnd();
    // Top wing outline
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx, cy + 25.0f * scale);
    glVertex2f(cx + xSign * (10.0f * scale), cy + 50.0f * scale);
    glVertex2f(cx + xSign * (30.0f * scale), cy + 40.0f * scale);
    glVertex2f(cx + xSign * (20.0f * scale), cy + 15.0f * scale);
    glEnd();
    // Bottom tail fin outline
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx + xSign * (40.0f * scale), cy - 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy - 25.0f * scale);
    glVertex2f(cx + xSign * (60.0f * scale), cy - 20.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy - 10.0f * scale);
    glEnd();
    // Top tail fin outline
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx + xSign * (40.0f * scale), cy + 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy + 25.0f * scale);
    glVertex2f(cx + xSign * (60.0f * scale), cy + 20.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy + 10.0f * scale);
    glEnd();
    // Engine outlines
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx + xSign * (40.0f * scale), cy - 10.0f * scale);
    glVertex2f(cx + xSign * (40.0f * scale), cy - 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy - 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy - 10.0f * scale);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(cx + xSign * (40.0f * scale), cy + 10.0f * scale);
    glVertex2f(cx + xSign * (40.0f * scale), cy + 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy + 15.0f * scale);
    glVertex2f(cx + xSign * (50.0f * scale), cy + 10.0f * scale);
    glEnd();
}

// Draw text at specified position
void drawText(float x, float y, const char* text)
{
    glColor3f(0.0f, 0.0f, 0.0f); // Black text
    glRasterPos2f(x, y);
    while (*text)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *text++);
    }
}

// Draw buildings on the far left with varied designs
void drawBuildings()
{
    const float buildingWidth = 50.0f; // Width of each building
    const int numBuildings = 5;        // Number of buildings
    const float buildingGap = 10.0f;   // Gap between buildings
    const float windowSize = 8.0f;     // Size of each window
    const float windowGap = 10.0f;     // Increased gap for fewer windows
    const float baseX = 0.0f;          // Start at left edge
    const float baseY = 0.0f;          // Bottom of screen

    // Building heights
    float buildingHeights[] = {450.0f, 350.0f, 300.0f, 400.0f, 320.0f};

    for (int i = 0; i < numBuildings; ++i)
    {
        float x = baseX + i * (buildingWidth + buildingGap);
        float height = buildingHeights[i];

        // Set building color to darker gray
        glColor3f(0.3f, 0.3f, 0.3f); // Dark gray (RGB: 77, 77, 77)

        // Draw building based on index for variety
        switch (i)
        {
            case 0: // Skyscraper with spire
            {
                // Main body
                glBegin(GL_QUADS);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height);
                glVertex2f(x, baseY + height);
                glEnd();
                // Spire
                glColor3f(0.2f, 0.2f, 0.2f); // Slightly darker for spire
                glBegin(GL_TRIANGLES);
                glVertex2f(x + buildingWidth / 2.0f, baseY + height + 50.0f); // Top
                glVertex2f(x + buildingWidth / 4.0f, baseY + height);         // Left base
                glVertex2f(x + 3.0f * buildingWidth / 4.0f, baseY + height); // Right base
                glEnd();
                // Outline main body
                glColor3f(0.0f, 0.0f, 0.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height);
                glVertex2f(x, baseY + height);
                glEnd();
                // Outline spire
                glBegin(GL_LINE_LOOP);
                glVertex2f(x + buildingWidth / 2.0f, baseY + height + 50.0f);
                glVertex2f(x + buildingWidth / 4.0f, baseY + height);
                glVertex2f(x + 3.0f * buildingWidth / 4.0f, baseY + height);
                glEnd();
                break;
            }
            case 1: // Mid-rise with setbacks
            {
                // Lower section
                glBegin(GL_QUADS);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height * 0.6f);
                glVertex2f(x, baseY + height * 0.6f);
                glEnd();
                // Upper section (narrower)
                glBegin(GL_QUADS);
                glVertex2f(x + buildingWidth * 0.2f, baseY + height * 0.6f);
                glVertex2f(x + buildingWidth * 0.8f, baseY + height * 0.6f);
                glVertex2f(x + buildingWidth * 0.8f, baseY + height);
                glVertex2f(x + buildingWidth * 0.2f, baseY + height);
                glEnd();
                // Outline
                glColor3f(0.0f, 0.0f, 0.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height * 0.6f);
                glVertex2f(x + buildingWidth * 0.8f, baseY + height * 0.6f);
                glVertex2f(x + buildingWidth * 0.8f, baseY + height);
                glVertex2f(x + buildingWidth * 0.2f, baseY + height);
                glVertex2f(x + buildingWidth * 0.2f, baseY + height * 0.6f);
                glVertex2f(x, baseY + height * 0.6f);
                glEnd();
                break;
            }
            case 2: // Short building with slanted roof
            {
                glBegin(GL_QUADS);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height * 0.7f);
                glVertex2f(x, baseY + height);
                glEnd();
                // Outline
                glColor3f(0.0f, 0.0f, 0.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height * 0.7f);
                glVertex2f(x, baseY + height);
                glEnd();
                break;
            }
            case 3: // Modern building with curved top
            {
                // Main body
                glBegin(GL_QUADS);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height);
                glVertex2f(x, baseY + height);
                glEnd();
                // Curved top (approximated with a semi-circle)
                glColor3f(0.2f, 0.2f, 0.2f); // Slightly darker for top
                glBegin(GL_TRIANGLE_FAN);
                float cx = x + buildingWidth / 2.0f;
                float cy = baseY + height;
                glVertex2f(cx, cy);
                for (int j = 0; j <= 50; ++j)
                {
                    float angle = 3.1415926f * float(j) / 50.0f; // 0 to pi
                    float wx = cx + (buildingWidth / 2.0f) * cosf(angle);
                    float wy = cy + 20.0f * sinf(angle);
                    glVertex2f(wx, wy);
                }
                glEnd();
                // Outline main body
                glColor3f(0.0f, 0.0f, 0.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height);
                glVertex2f(x, baseY + height);
                glEnd();
                // Outline curved top
                glBegin(GL_LINE_STRIP);
                for (int j = 0; j <= 50; ++j)
                {
                    float angle = 3.1415926f * float(j) / 50.0f;
                    float wx = cx + (buildingWidth / 2.0f) * cosf(angle);
                    float wy = cy + 20.0f * sinf(angle);
                    glVertex2f(wx, wy);
                }
                glEnd();
                break;
            }
            case 4: // Classic building with triangular pediment
            {
                // Main body
                glBegin(GL_QUADS);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height);
                glVertex2f(x, baseY + height);
                glEnd();
                // Pediment
                glColor3f(0.2f, 0.2f, 0.2f); // Slightly darker for pediment
                glBegin(GL_TRIANGLES);
                glVertex2f(x, baseY + height);
                glVertex2f(x + buildingWidth, baseY + height);
                glVertex2f(x + buildingWidth / 2.0f, baseY + height + 30.0f);
                glEnd();
                // Outline main body
                glColor3f(0.0f, 0.0f, 0.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2f(x, baseY);
                glVertex2f(x + buildingWidth, baseY);
                glVertex2f(x + buildingWidth, baseY + height);
                glVertex2f(x, baseY + height);
                glEnd();
                // Outline pediment
                glBegin(GL_LINE_LOOP);
                glVertex2f(x, baseY + height);
                glVertex2f(x + buildingWidth, baseY + height);
                glVertex2f(x + buildingWidth / 2.0f, baseY + height + 30.0f);
                glEnd();
                break;
            }
        }

        // Draw yellow windows (fewer, with larger gaps)
        glColor3f(1.0f, 1.0f, 0.0f); // Yellow (RGB: 255, 255, 0)
        // Calculate number of windows based on building type
        float windowHeight = height; // Default height for window placement
        float windowXOffset = 0.0f; // Adjust for setbacks or other shapes
        float windowWidth = buildingWidth;
        if (i == 1) // Setback building
        {
            // Windows only in lower section for simplicity
            windowHeight = height * 0.6f;
            windowXOffset = 0.0f;
            windowWidth = buildingWidth;
        }
        else if (i == 2) // Slanted roof
        {
            // Adjust window height to avoid roof
            windowHeight = height * 0.7f;
        }

        int numWindowsX = static_cast<int>((windowWidth - windowGap) / (windowSize + windowGap));
        int numWindowsY = static_cast<int>((windowHeight - windowGap) / (windowSize + windowGap));
        for (int wy = 0; wy < numWindowsY; ++wy)
        {
            for (int wx = 0; wx < numWindowsX; ++wx)
            {
                float windowX = x + windowGap + windowXOffset + wx * (windowSize + windowGap);
                float windowY = baseY + windowGap + wy * (windowSize + windowGap);
                // For slanted roof, ensure windows don't cross the roof line
                if (i == 2)
                {
                    float roofYAtX = baseY + height - (windowX - x) * (height - height * 0.7f) / buildingWidth;
                    if (windowY + windowSize > roofYAtX)
                        continue;
                }
                glBegin(GL_QUADS);
                glVertex2f(windowX, windowY);
                glVertex2f(windowX + windowSize, windowY);
                glVertex2f(windowX + windowSize, windowY + windowSize);
                glVertex2f(windowX, windowY + windowSize);
                glEnd();
            }
        }
    }
}

// Draw notation boxes with text and triangle arrows
void drawNotations()
{
    // Box dimensions
    const float boxWidth = 100.0f;
    const float boxHeight = 30.0f;
    const float boxY = 50.0f; // Bottom of screen
    const float box1X = 50.0f; // First box (Fire)
    const float box2X = 160.0f; // Second box (Hold Fire, 10px gap)
    const float box3X = 270.0f; // Third box (Reset, 10px gap)

    // Draw white background for boxes
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glBegin(GL_QUADS);
    // Fire box
    glVertex2f(box1X, boxY);
    glVertex2f(box1X + boxWidth, boxY);
    glVertex2f(box1X + boxWidth, boxY + boxHeight);
    glVertex2f(box1X, boxY + boxHeight);
    // Hold Fire box
    glVertex2f(box2X, boxY);
    glVertex2f(box2X + boxWidth, boxY);
    glVertex2f(box2X + boxWidth, boxY + boxHeight);
    glVertex2f(box2X, boxY + boxHeight);
    // Reset box
    glVertex2f(box3X, boxY);
    glVertex2f(box3X + boxWidth, boxY);
    glVertex2f(box3X + boxWidth, boxY + boxHeight);
    glVertex2f(box3X, boxY + boxHeight);
    glEnd();

    // Draw black border for boxes
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINE_LOOP);
    glVertex2f(box1X, boxY);
    glVertex2f(box1X + boxWidth, boxY);
    glVertex2f(box1X + boxWidth, boxY + boxHeight);
    glVertex2f(box1X, boxY + boxHeight);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(box2X, boxY);
    glVertex2f(box2X + boxWidth, boxY);
    glVertex2f(box2X + boxWidth, boxY + boxHeight);
    glVertex2f(box2X, boxY + boxHeight);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(box3X, boxY);
    glVertex2f(box3X + boxWidth, boxY);
    glVertex2f(box3X + boxWidth, boxY + boxHeight);
    glVertex2f(box3X, boxY + boxHeight);
    glEnd();

    // Draw triangles (left for Fire, right for Hold Fire, none for Reset)
    const float triangleSize = 10.0f; // Triangle width and height
    const float triangleYCenter = boxY + boxHeight / 2.0f;
    const float triangleX1 = box1X + 10.0f; // Triangle position in Fire box
    const float triangleX2 = box2X + 10.0f; // Triangle position in Hold Fire box

    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_TRIANGLES);
    // Left triangle for Fire
    glVertex2f(triangleX1, triangleYCenter); // Right center
    glVertex2f(triangleX1 + triangleSize / 2.0f, triangleYCenter + triangleSize / 2.0f); // Top left
    glVertex2f(triangleX1 + triangleSize / 2.0f, triangleYCenter - triangleSize / 2.0f); // Bottom left
    // Right triangle for Hold Fire
    glVertex2f(triangleX2 + triangleSize / 2.0f, triangleYCenter); // Left center
    glVertex2f(triangleX2, triangleYCenter + triangleSize / 2.0f); // Top right
    glVertex2f(triangleX2, triangleYCenter - triangleSize / 2.0f); // Bottom right
    glEnd();

    // Draw text ("Fire", "Hold Fire", and "R Reset")
    const char* fireText = "Fire";
    const char* holdText = "Hold Fire";
    const char* resetText = "R Reset";
    float textHeight = 12.0f; // Approximate height of Helvetica 12
    float textY = boxY + (boxHeight - textHeight) / 2.0f + 3.0f; // Center vertically
    // Text starts after triangle for Fire and Hold Fire, centered for Reset
    float fireTextX = triangleX1 + triangleSize + 5.0f;
    float holdTextX = triangleX2 + triangleSize + 5.0f;
    float resetTextX = box3X + (boxWidth - 50.0f) / 2.0f; // Approximate centering (50px text width)
    drawText(fireTextX, textY, fireText);
    drawText(holdTextX, textY, holdText);
    drawText(resetTextX, textY, resetText);
}

// Draw scoreboards in top right corner with bold text
void drawScoreboard()
{
    // Box dimensions for KILLS
    const float killsBoxWidth = 100.0f;
    const float killsBoxHeight = 40.0f;
    const float killsBoxX = 1080.0f; // 1200 - 100 - 20 (right edge - width - padding)
    const float killsBoxY = 940.0f;  // 1000 - 40 - 20 (top edge - height - padding)
    // Box dimensions for Bullets
    const float bulletsBoxWidth = 140.0f;
    const float bulletsBoxHeight = 40.0f;
    const float bulletsBoxX = 930.0f; // killsBoxX - 100 - 10 (left of KILLS with 10px gap)
    const float bulletsBoxY = 940.0f; // Same y as KILLS
    // Box dimensions for Missiles
    const float missilesBoxWidth = 140.0f;
    const float missilesBoxHeight = 40.0f;
    const float missilesBoxX = 780.0f; // bulletsBoxX - 140 - 10 (left of BULLETS with 10px gap)
    const float missilesBoxY = 940.0f; // Same y as BULLETS
    // Box dimensions for Life
    const float lifeBoxWidth = 100.0f;
    const float lifeBoxHeight = 40.0f;
    const float lifeBoxX = 670.0f; // missilesBoxX - 100 - 10 (left of MISSILES with 10px gap)
    const float lifeBoxY = 940.0f; // Same y as others

    // Draw white background for boxes
    glColor3f(1.0f, 1.0f, 1.0f); // White
    glBegin(GL_QUADS);
    // KILLS box
    glVertex2f(killsBoxX, killsBoxY);
    glVertex2f(killsBoxX + killsBoxWidth, killsBoxY);
    glVertex2f(killsBoxX + killsBoxWidth, killsBoxY + killsBoxHeight);
    glVertex2f(killsBoxX, killsBoxY + killsBoxHeight);
    // Bullets box
    glVertex2f(bulletsBoxX, bulletsBoxY);
    glVertex2f(bulletsBoxX + bulletsBoxWidth, bulletsBoxY);
    glVertex2f(bulletsBoxX + bulletsBoxWidth, bulletsBoxY + bulletsBoxHeight);
    glVertex2f(bulletsBoxX, bulletsBoxY + bulletsBoxHeight);
    // Missiles box
    glVertex2f(missilesBoxX, missilesBoxY);
    glVertex2f(missilesBoxX + missilesBoxWidth, missilesBoxY);
    glVertex2f(missilesBoxX + missilesBoxWidth, missilesBoxY + missilesBoxHeight);
    glVertex2f(missilesBoxX, missilesBoxY + missilesBoxHeight);
    // Life box
    glVertex2f(lifeBoxX, lifeBoxY);
    glVertex2f(lifeBoxX + lifeBoxWidth, lifeBoxY);
    glVertex2f(lifeBoxX + lifeBoxWidth, lifeBoxY + lifeBoxHeight);
    glVertex2f(lifeBoxX, lifeBoxY + lifeBoxHeight);
    glEnd();

    // Draw black border for boxes
    glColor3f(0.0f, 0.0f, 0.0f); // Black
    glBegin(GL_LINE_LOOP);
    glVertex2f(killsBoxX, killsBoxY);
    glVertex2f(killsBoxX + killsBoxWidth, killsBoxY);
    glVertex2f(killsBoxX + killsBoxWidth, killsBoxY + killsBoxHeight);
    glVertex2f(killsBoxX, killsBoxY + killsBoxHeight);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(bulletsBoxX, bulletsBoxY);
    glVertex2f(bulletsBoxX + bulletsBoxWidth, bulletsBoxY);
    glVertex2f(bulletsBoxX + bulletsBoxWidth, bulletsBoxY + bulletsBoxHeight);
    glVertex2f(bulletsBoxX, bulletsBoxY + bulletsBoxHeight);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(missilesBoxX, missilesBoxY);
    glVertex2f(missilesBoxX + missilesBoxWidth, missilesBoxY);
    glVertex2f(missilesBoxX + missilesBoxWidth, missilesBoxY + missilesBoxHeight);
    glVertex2f(missilesBoxX, missilesBoxY + missilesBoxHeight);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex2f(lifeBoxX, lifeBoxY);
    glVertex2f(lifeBoxX + lifeBoxWidth, lifeBoxY);
    glVertex2f(lifeBoxX + lifeBoxWidth, lifeBoxY + lifeBoxHeight);
    glVertex2f(lifeBoxX, lifeBoxY + lifeBoxHeight);
    glEnd();

    // Draw bold text "KILLS: <number>"
    char killsText[20];
    snprintf(killsText, sizeof(killsText), "KILLS: %d", kills);
    float killsTextHeight = 18.0f; // Approximate height of Helvetica 18
    float killsTextX = killsBoxX + 10.0f; // 10px padding from left edge
    float killsTextY = killsBoxY + (killsBoxHeight - killsTextHeight) / 2.0f + 5.0f; // Center vertically
    glColor3f(0.0f, 0.0f, 0.0f); // Black text
    glRasterPos2f(killsTextX, killsTextY);
    for (const char* c = killsText; *c; ++c)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // Draw bold text "BULLETS: <number>"
    char bulletsText[20];
    snprintf(bulletsText, sizeof(bulletsText), "BULLETS: %d", bullets);
    float bulletsTextHeight = 18.0f; // Approximate height of Helvetica 18
    float bulletsTextX = bulletsBoxX + 10.0f; // 10px padding from left edge
    float bulletsTextY = bulletsBoxY + (bulletsBoxHeight - bulletsTextHeight) / 2.0f + 5.0f; // Center vertically
    glRasterPos2f(bulletsTextX, bulletsTextY);
    for (const char* c = bulletsText; *c; ++c)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // Draw bold text "MISSILES: <number>"
    char missilesText[20];
    snprintf(missilesText, sizeof(missilesText), "MISSILES: %d", missilesCount);
    float missilesTextHeight = 18.0f; // Approximate height of Helvetica 18
    float missilesTextX = missilesBoxX + 10.0f; // 10px padding from left edge
    float missilesTextY = missilesBoxY + (missilesBoxHeight - missilesTextHeight) / 2.0f + 5.0f; // Center vertically
    glRasterPos2f(missilesTextX, missilesTextY);
    for (const char* c = missilesText; *c; ++c)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }

    // Draw bold text "LIFE: <number>"
    char lifeText[20];
    snprintf(lifeText, sizeof(lifeText), "LIFE: %d", lives);
    float lifeTextHeight = 18.0f; // Approximate height of Helvetica 18
    float lifeTextX = lifeBoxX + 10.0f; // 10px padding from left edge
    float lifeTextY = lifeBoxY + (lifeBoxHeight - lifeTextHeight) / 2.0f + 5.0f; // Center vertically
    glRasterPos2f(lifeTextX, lifeTextY);
    for (const char* c = lifeText; *c; ++c)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
    }
}

// Draw game over screen with final kill score in the center
void drawGameOver()
{
    // Semi-transparent black overlay
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Black with 50% opacity
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(1200.0f, 0.0f);
    glVertex2f(1200.0f, 1000.0f);
    glVertex2f(0.0f, 1000.0f);
    glEnd();

    // Draw final kills in center with larger font
    char finalText[20];
    snprintf(finalText, sizeof(finalText), "KILLS: %d", finalKills);
    glColor3f(1.0f, 1.0f, 1.0f); // White text
    glRasterPos2f(600.0f - 50.0f, 500.0f); // Approximate center (1200/2, 1000/2), adjusted for text width
    for (const char* c = finalText; *c; ++c)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); // Larger font
    }
}

// Initialize OpenGL state
void init(void)
{
    glClearColor(0.0f, 0.5f, 1.0f, 1.0f); // Vivid blue sky (RGB: 0, 128, 255)
    glEnable(GL_BLEND); // Enable alpha blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Set blending function
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1200, 0, 1000); // 1200x1000 viewport
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // Seed random number generator for enemy y-positions
    srand(static_cast<unsigned>(time(nullptr)));
}

// Reset game state
void resetGame()
{
    projectiles.clear(); // Remove all projectiles
    missiles.clear();    // Remove all missiles
    enemies.clear();     // Remove all enemy planes
    fireballs.clear();   // Remove all fireballs
    kills = 0;           // Reset kill count
    bullets = 100;       // Reset bullet count
    missilesCount = 5;   // Reset missile count
    lives = 3;           // Reset lives to 3
    planeY = 550.0f;     // Reset player plane to initial position
    isFiring = false;    // Stop firing projectiles
    isFiringMissile = false; // Stop firing missiles
    moveUp = false;      // Stop upward movement
    moveDown = false;    // Stop downward movement
    frameCount = 0;      // Reset frame counter for consistent timing
    showGameOver = false; // Clear game over state
    gameOverTimer = 0;   // Reset game over timer
    blinkActive = false; // Reset blinking
    blinkCount = 0;
    blinkPhase = 0;
}

// Keyboard callback for special keys (arrow keys)
void specialKeys(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_LEFT:
            isFiring = true; // Enable firing projectiles
            break;
        case GLUT_KEY_RIGHT:
            isFiring = false; // Disable firing projectiles
            break;
        case GLUT_KEY_UP:
            moveUp = true; // Start moving up
            break;
        case GLUT_KEY_DOWN:
            moveDown = true; // Start moving down
            break;
    }
}

// Keyboard release callback for special keys
void specialKeysUp(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_UP:
            moveUp = false; // Stop moving up
            break;
        case GLUT_KEY_DOWN:
            moveDown = false; // Stop moving down
            break;
    }
}

// Keyboard callback for regular keys
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'r':
        case 'R': // Case-insensitive 'R' key
            resetGame();
            break;
        case 'm':
        case 'M': // Case-insensitive 'M' key
            if (missilesCount > 0) // Only allow firing if missiles are available
                isFiringMissile = true; // Trigger missile firing
            break;
    }
}

// Check for collisions between projectiles/missiles and enemy planes, and player-enemy collisions
void checkCollisions()
{
    // Check projectiles
    for (auto& proj : projectiles)
    {
        if (!proj.active)
            continue;
        for (auto& enemy : enemies)
        {
            if (!enemy.active)
                continue;
            // Enemy plane bounding box (approximated for 50% scale)
            float enemyWidth = 40.0f * 0.5f; // Half of full width (80 * 0.5)
            float enemyHeight = 50.0f * 0.5f; // Half of full height (100 * 0.5)
            float left = enemy.x - enemyWidth;
            float right = enemy.x + enemyWidth;
            float top = enemy.y + enemyHeight;
            float bottom = enemy.y - enemyHeight;
            // Check if projectile is within enemy bounding box
            if (proj.x >= left && proj.x <= right && proj.y >= bottom && proj.y <= top)
            {
                proj.active = false; // Deactivate projectile
                enemy.active = false; // Deactivate enemy
                kills++; // Increment kill count
            }
        }
    }

    // Check missiles
    for (auto& missile : missiles)
    {
        if (!missile.active)
            continue;
        for (auto& enemy : enemies)
        {
            if (!enemy.active)
                continue;
            // Enemy plane bounding box (approximated for 50% scale)
            float enemyWidth = 40.0f * 0.5f;
            float enemyHeight = 50.0f * 0.5f;
            float left = enemy.x - enemyWidth;
            float right = enemy.x + enemyWidth;
            float top = enemy.y + enemyHeight;
            float bottom = enemy.y - enemyHeight;
            // Missile bounding box (approximated)
            float missileLength = 40.0f; // Body length + nose length
            float missileHeight = 8.0f;  // Body height
            float missileLeft = missile.x - missileLength;
            float missileRight = missile.x;
            float missileTop = missile.y + missileHeight / 2.0f;
            float missileBottom = missile.y - missileHeight / 2.0f;
            // Check for overlap between missile and enemy
            if (missileRight >= left && missileLeft <= right && missileTop >= bottom && missileBottom <= top)
            {
                missile.active = false; // Deactivate missile
                // Create fireball at missile position
                fireballs.emplace_back(missile.x, missile.y);
                // Check all enemies for explosion radius
                for (auto& e : enemies)
                {
                    if (!e.active)
                        continue;
                    // Calculate distance from missile to enemy
                    float dx = e.x - missile.x;
                    float dy = e.y - missile.y;
                    float distance = sqrtf(dx * dx + dy * dy);
                    // If within explosion radius, deactivate enemy
                    if (distance <= MISSILE_EXPLOSION_RADIUS)
                    {
                        e.active = false;
                        kills++; // Increment kill count for each destroyed enemy
                    }
                }
                break; // Stop checking this missile after it hits
            }
        }
    }

    // Check player-enemy collisions
    for (auto& enemy : enemies)
    {
        if (!enemy.active)
            continue;
        // Player plane bounding box (full scale)
        float playerWidth = 40.0f; // Half of full width (80 * 1.0)
        float playerHeight = 50.0f; // Half of full height (100 * 1.0)
        float playerLeft = 1050.0f - playerWidth; // 1010
        float playerRight = 1050.0f + playerWidth; // 1090
        float playerTop = planeY + playerHeight;
        float playerBottom = planeY - playerHeight;
        // Enemy plane bounding box (50% scale)
        float enemyWidth = 40.0f * 0.5f; // 20
        float enemyHeight = 50.0f * 0.5f; // 25
        float enemyLeft = enemy.x - enemyWidth;
        float enemyRight = enemy.x + enemyWidth;
        float enemyTop = enemy.y + enemyHeight;
        float enemyBottom = enemy.y - enemyHeight;
        // Check for overlap
        if (playerRight >= enemyLeft && playerLeft <= enemyRight &&
            playerTop >= enemyBottom && playerBottom <= enemyTop)
        {
            enemy.active = false; // Deactivate enemy to prevent multiple hits
            lives--; // Decrease life count
            // Start blinking effect
            blinkActive = true;
            blinkCount = 0;
            blinkPhase = 0;
            if (lives == 0)
            {
                finalKills = kills; // Store final kills
                showGameOver = true; // Trigger game over
                gameOverTimer = 0; // Start timer
            }
        }
    }
}

// Update game state: player plane, projectiles, missiles, fireballs, enemy planes, and blinking
void updateGame()
{
    if (showGameOver)
    {
        gameOverTimer++;
        if (gameOverTimer >= 120) // ~2 seconds at 60 FPS
        {
            resetGame();
        }
        return; // Skip normal game updates during game over
    }

    // Update player plane position
    if (moveUp && planeY < PLANE_Y_MAX)
        planeY += PLANE_SPEED;
    if (moveDown && planeY > PLANE_Y_MIN)
        planeY -= PLANE_SPEED;

    // Update blinking effect
    if (blinkActive)
    {
        blinkCount++;
        if (blinkCount >= 5) // Each phase lasts 5 frames (~83ms)
        {
            blinkCount = 0; // Reset frame counter
            blinkPhase++;   // Move to next phase
            if (blinkPhase >= 4) // After two blinks (off-on-off-on)
            {
                blinkActive = false; // Stop blinking
                blinkPhase = 0;     // Reset phase
            }
        }
    }

    // Update existing projectiles
    for (auto& proj : projectiles)
    {
        if (proj.active)
        {
            proj.x -= PROJECTILE_SPEED; // Move left
            if (proj.x < 0)            // Off-screen
                proj.active = false;
        }
    }

    // Remove inactive projectiles
    projectiles.erase(
        std::remove_if(projectiles.begin(), projectiles.end(),
                       [](const Projectile& p) { return !p.active; }),
        projectiles.end());

    // Update existing missiles
    for (auto& missile : missiles)
    {
        if (missile.active)
        {
            missile.x -= PROJECTILE_SPEED; // Move left at same speed as projectiles
            if (missile.x < 0)             // Off-screen
                missile.active = false;
        }
    }

    // Remove inactive missiles
    missiles.erase(
        std::remove_if(missiles.begin(), missiles.end(),
                       [](const Missile& m) { return !m.active; }),
        missiles.end());

    // Update existing fireballs
    for (auto& fireball : fireballs)
    {
        if (fireball.duration > 0)
        {
            fireball.duration--; // Decrease duration
        }
    }

    // Remove expired fireballs
    fireballs.erase(
        std::remove_if(fireballs.begin(), fireballs.end(),
                       [](const Fireball& f) { return f.duration <= 0; }),
        fireballs.end());

    // Update existing enemy planes
    for (auto& enemy : enemies)
    {
        if (enemy.active)
        {
            enemy.x += ENEMY_SPEED; // Move right
            if (enemy.x > 1200)     // Off-screen
                enemy.active = false;
        }
    }

    // Remove inactive enemy planes
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
                       [](const EnemyPlane& e) { return !e.active; }),
        enemies.end());

    // Check for collisions
    checkCollisions();

    // Spawn new projectile every SHOOT_INTERVAL frames if firing and bullets available
    if (isFiring && bullets > 0)
    {
        frameCount++;
        if (frameCount >= SHOOT_INTERVAL)
        {
            // Player plane nose at (1010, planeY)
            projectiles.emplace_back(1010.0f, planeY);
            bullets--; // Decrease bullet count
            frameCount = 0; // Reset counter
            if (bullets == 0 && missilesCount == 0)
            {
                finalKills = kills; // Store final kills
                showGameOver = true; // Trigger game over
                gameOverTimer = 0; // Start timer
            }
        }
    }
    // Spawn a missile when 'M' is pressed and missiles are available
    else if (isFiringMissile && missilesCount > 0)
    {
        frameCount++;
        if (frameCount >= SHOOT_INTERVAL)
        {
            // Player plane nose at (1010, planeY)
            missiles.emplace_back(1010.0f, planeY);
            missilesCount--; // Decrease missile count
            frameCount = 0; // Reset counter
            isFiringMissile = false; // Reset missile firing state
            if (bullets == 0 && missilesCount == 0)
            {
                finalKills = kills; // Store final kills
                showGameOver = true; // Trigger game over
                gameOverTimer = 0; // Start timer
            }
        }
    }
    else
    {
        frameCount++;
    }

    // Spawn new enemy plane every ENEMY_SPAWN_INTERVAL frames
    if (frameCount % ENEMY_SPAWN_INTERVAL == 0)
    {
        // Random y-position between PLANE_Y_MIN and PLANE_Y_MAX
        float y = PLANE_Y_MIN + static_cast<float>(rand()) / RAND_MAX * (PLANE_Y_MAX - PLANE_Y_MIN);
        enemies.emplace_back(0.0f, y); // Spawn at left edge
    }
}

// Timer callback for animation
void timer(int value)
{
    updateGame();
    glutPostRedisplay(); // Request redraw
    glutTimerFunc(16, timer, 0); // ~60 FPS (1000ms / 60 = 16ms)
}

// Display callback
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT); // Clear to blue sky
    glLoadIdentity();

    // Draw buildings (far background)
    drawBuildings();

    // Draw clouds as simple circles (white, 60% opacity)
    drawCircle(200.0f, 850.0f, 40.0f, 1); // Cloud 1: Left side
    drawCircle(500.0f, 900.0f, 50.0f, 1); // Cloud 2: Middle-left
    drawCircle(800.0f, 800.0f, 45.0f, 1); // Cloud 3: Middle-right
    drawCircle(1000.0f, 870.0f, 35.0f, 1); // Cloud 4: Right side

    // Draw fireballs
    for (const auto& fireball : fireballs)
    {
        if (fireball.duration > 0)
            drawCircle(fireball.x, fireball.y, FIREBALL_RADIUS, 2); // Fireball type
    }

    // Draw enemy planes (right-facing, 50% scale)
    if (!showGameOver)
    {
        for (const auto& enemy : enemies)
        {
            if (enemy.active)
                drawWarPlane(enemy.x, enemy.y, 0.5f, true); // Scale=0.5, right-facing
        }

        // Draw player F-22 Raptor on right mid side, facing left, only if not in "off" blink phase
        if (!blinkActive || blinkPhase == 1 || blinkPhase == 3)
        {
            drawWarPlane(1050.0f, planeY, 1.0f, false); // Position: x=1050, y=planeY, full scale, left-facing
        }

        // Draw active projectiles (light red, fully opaque)
        for (const auto& proj : projectiles)
        {
            if (proj.active)
                drawCircle(proj.x, proj.y, PROJECTILE_RADIUS, 0); // Projectile type
        }

        // Draw active missiles
        for (const auto& missile : missiles)
        {
            if (missile.active)
                drawMissile(missile.x, missile.y);
        }
    }

    // Draw notation boxes
    drawNotations();

    // Draw scoreboards
    drawScoreboard();

    // Draw game over screen if active
    if (showGameOver)
    {
        drawGameOver();
    }

    glutSwapBuffers(); // Swap buffers for double buffering
}

// Main function
int main(int argc, char** argv)
{
    glutInit(&argc, argv); // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // Double buffering, RGB color
    glutInitWindowSize(1200, 1000); // Window size
    glutInitWindowPosition(50, 50); // Window position
    glutCreateWindow("Blue Sky with Clouds, Left-Facing F-22 Raptor, and Keyboard-Controlled Shooting"); // Window title
    init(); // Initialize OpenGL settings
    glutDisplayFunc(display); // Register display callback
    glutSpecialFunc(specialKeys); // Register special key press callback
    glutSpecialUpFunc(specialKeysUp); // Register special key release callback
    glutKeyboardFunc(keyboard); // Register regular key press callback
    glutTimerFunc(0, timer, 0); // Start timer for animation
    glutMainLoop(); // Enter GLUT event loop
    return 0; // Unreachable, included for completeness
}