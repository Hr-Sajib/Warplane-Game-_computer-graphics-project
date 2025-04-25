#include <GLUT/glut.h>
#include <stdio.h>
#include <math.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <cmath>

// --- Data Definitions ---

// Grass Data
float grass_scale[] = {0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};
float grass_cx[] = {150.0f, 370.0f, 570.0f, 690.0f, 930.0f, 1100.0f,780.0f};
float grass_cy[] = {100.0f, 150.0f, 350.0f, 300.0f, 250.0f, 200.0f, 100.0f};

// Oyster Data
GLfloat oyster_scale[] = {0.2f, 0.2f, 0.2f};
GLfloat oyster_cx[] = {150.0f, 550.0f, 1150.0f};
GLfloat oyster_cy[] = {350.0f, 320.0f, 360.0f};

// Starfish Data
float star_scale[] = {0.1f, 0.1f, 0.1f};
float starcx[] = {350.0f, 700.0f, 950.0f};
float starcy[] = {350.0f, 380.0f, 370.0f};

// Sandhouse Data
float sandhouse_scale = 0.2f;
float sandhouse_X = 400.0f;
float sandhouse_Y = 50.0f;

// Handle Data
float handle_X = 475.0f;
float handle_Y = 50.0f;
float handle_scale = 0.5f;

// Ball Data
struct Ball
{
    float x, y;
    float scale;
    float r, g, b;
    Ball(float x, float y, float scale, float r, float g, float b): x(x), y(y), scale(scale), r(r), g(g),b(b){}
};

std::vector<Ball> balls =
{
    Ball(300.0f, 100.0f, 0.5f, 1.0f, 0.9f, 1.0f),
    Ball(1080.0f, 340.0f, 0.5f, 1.0f, 0.8f, 0.9f)
};


// Signboard Data
float signboard_X = 830.0f;
float signboard_Y = 0.0f;
float signboard_scale = 1.0f;
const char* signboard_upperText = ".....beach";
const char* signboard_lowerText1 = "Created by:";
const char* signboard_lowerText2 = "Dipu &";
const char* signboard_lowerText4 = "Rayhan";

// Surfboard Data
float surfboard_X = 650.0f;
float surfboard_Y = 150.0f;
float surfboard_scale = 0.6f;

// User Boat Data
float boat_X = 800.0f;
float boat_Y = 500.0f;
float boat_scale = 1.0f;
float boat_move_speed = 7.0f;
float boat_left_margin = -100.0f;
float boat_right_margin = 950.0f;

// Stand (Watch Tower) Data
float stand_X = 900.0f;
float stand_Y = 150.0f;
float stand_scale = 0.65f;

// Umbrella Data
float umbrella_X = 330.0f;
float umbrella_Y = 415.0f;
float umbrella_scale = 0.6f;

// Beach Seat Data
float beachSeat_X = 190.0f;
float beachSeat_Y = 250.0f;
float beachSeat_scale = 0.5f;

// Cloud Data
float cloud_X = 0.0f;
float cloud_Y = 0.0f;
float cloud_scale = 1.0f;

// Sun/Moon Data
float sun_X = 600.0f;
float sun_Y = 925.0f;
float sun_baseRadius = 40.0f;
float sun_currentRadius = sun_baseRadius;
float sun_pulseScale = 0.8f;
float sun_pulseDirection = 0.002f;
float sun_minScale = 0.95f;
float sun_maxScale = 1.05f;
GLfloat sun_color_r = 1.0f;
GLfloat sun_color_g = 1.0f;
GLfloat sun_color_b = 0.0f;
GLfloat sun_halo_color_r = 1.0f;
GLfloat sun_halo_color_g = 1.0f;
GLfloat sun_halo_color_b = 0.5f;

// Moving Boat Data
float movingBoat_X = 1300.0f;
float movingBoat_Y = 450.0f;
float movingBoat_scale = 0.8f;

// Tree Data - MODIFIED: Using single master scale
const int NUM_TREES = 4;
float tree_master_scale = 1.5f; // Master scale for all trees
float tree_cx[] = {50.0f, 100.0f, 160.0f, 890.0f};
float tree_cy[] = {250.0f,180.0f,0.0f, 300.0f};

// Tortoise Data
float tortoise_X = 550.0f; // X position fixed
float tortoise_Y = 300.0f; // Y position controllable
float tortoise_scale = 0.45f;
float tortoise_move_speed = 5.0f;
float tortoise_Y_min = 50.0f;
float tortoise_Y_max_disappear = 395.0f;
bool isTortoiseVisible = true;

// Day/Night Mode State
bool isNightMode = false;

// Wave Animation Data
const float WATER_LEVEL_Y = 400.0f; // Consistent water level
float waveAmplitude = 4.0f;
float waveFrequency = 0.015f;
float wavePhase = 0.0f;
const int waveSegments = 100;

// Star Data
const int NUM_STARS = 150;
float starPositions[NUM_STARS][2];

// Dolphin Animation Data
const float GRAVITY = 0.15f;
struct Dolphin
{
    float x, y;             // Position
    float speedX, speedY;   // Velocity components
    float angle;            // Rotation angle based on trajectory
    bool visible;           // Is the dolphin currently jumping?
    float timeSinceLastJump;
    float jumpInterval;     // Time between jumps (randomized)
    float scale;
    float initialJumpSpeedY;
    float initialJumpSpeedX;
    
Dolphin(float startX, float startY, float s, float interval) : x(startX), y(startY), speedX(0), speedY(0), angle(0),
          visible(false), timeSinceLastJump(0), jumpInterval(interval), scale(s),
          initialJumpSpeedY(6.0f + (float)(rand()%30)/10.0f),
    // Base jump height + random variation
          initialJumpSpeedX( ((rand()%2 == 0) ? 1.0f : -1.0f) * (1.5f + (float)(rand()%10)/10.0f) )
    // Random direction and speed
    
    {}
    void startJump()
    {
        visible = true;
        y = WATER_LEVEL_Y; // Start exactly at water level
        
        x += initialJumpSpeedX * 5; // Start slightly ahead based on direction
        speedY = initialJumpSpeedY; // Use the randomized initial Y speed
        speedX = initialJumpSpeedX; // Use the randomized initial X speed
        angle = atan2(speedY, speedX) * 180.0f / 3.1415926f; // Initial angle
    }

    void update(float dt)   // dt is time delta, assume 1 for simplicity now
    {
        if (visible)
        {
            x += speedX * dt;
            y += speedY * dt;
            speedY -= GRAVITY * dt; // Apply gravity
            angle = atan2(speedY, speedX) * 180.0f / 3.1415926f; // Update angle

            // Check if dolphin re-entered water
            if (y < WATER_LEVEL_Y && speedY < 0)
            {
                visible = false;
                y = WATER_LEVEL_Y; // Reset position
                timeSinceLastJump = 0;
                // Reset position slightly off-screen or based on speedX if needed
                if (x < 0 || x > 1200) x = 400 + (rand()%600); //Reposition if off-screen within the modified range
                // Randomize next jump interval and speeds
                jumpInterval = 70.0f + (rand() % 180); // Use updated interval range
                initialJumpSpeedY = 6.0f + (float)(rand()%30)/10.0f;
                initialJumpSpeedX = ((rand()%2 == 0) ? 1.0f : -1.0f) * (1.5f + (float)(rand()%10)/10.0f);
            }
        }
        else
        {
            timeSinceLastJump += dt;
            if (timeSinceLastJump >= jumpInterval)
            {
                startJump();
            }
        }
    }
};
std::vector<Dolphin> dolphins;
const int NUM_DOLPHINS = 2; // Number of dolphins

// Shadow Color
GLfloat shadowColor[] = {0.15f, 0.15f, 0.15f, 0.5f}; // Dark grey, semi-transparent

// --- Drawing Helper Functions ---

void drawEllipseSector(GLfloat rx, GLfloat ry, GLfloat cx, GLfloat cy, GLfloat fangle, GLfloat langle)
{
    glBegin(GL_POLYGON);
    glVertex2f(cx, cy);
    for (GLfloat i = fangle; i <= langle; i++)
    {
        GLfloat angle = i * 3.1415926f / 180.0f;
        GLfloat x = rx * cosf(angle);
        GLfloat y = ry * sinf(angle);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();

}

void drawStarfish(float scale, float cx, float cy)
{
    if (isNightMode) glColor3f(0.6f, 0.45f, 0.1f); // Darker orange/brown
    else glColor3f(0.8f, 0.6f, 0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(cx-150*scale, cy-200*scale);
    glVertex2f(cx-20*scale, cy-110*scale);
    glVertex2f(cx-130*scale, cy-110*scale);
    glEnd();
    glBegin(GL_TRIANGLES);
    glVertex2f(cx-130*scale, cy-110*scale);
    glVertex2f(cx, cy-200*scale);
    glVertex2f(cx-20*scale, cy-110*scale);
    glEnd();
    glBegin(GL_POLYGON);
    glVertex2f(cx-130*scale, cy-110*scale);
    glVertex2f(cx-20*scale, cy-110*scale);
    glVertex2f(cx+50*scale, cy-50*scale);
    glVertex2f(cx-50*scale, cy-50*scale);
    glVertex2f(cx-80*scale, cy+20*scale);
    glVertex2f(cx-100*scale, cy-50*scale);
    glVertex2f(cx-200*scale, cy-50*scale);
    glEnd();
}

void drawGrass()
{
    int num_grass = sizeof(grass_scale)/sizeof(grass_scale[0]);
    if (isNightMode) glColor3f(0.0f, 0.35f, 0.05f); // Darker, less saturated green
    else glColor3f(0.0f, 0.6f, 0.0f);
    for (int i=0; i<num_grass; i++)
    {
        glBegin(GL_POLYGON);
        glVertex2f(grass_cx[i]+grass_scale[i]*0.0f, grass_cy[i]+grass_scale[i]*170.0f);
        glVertex2f(grass_cx[i]+grass_scale[i]*100.0f, grass_cy[i]+grass_scale[i]*250.0f);
        glVertex2f(grass_cx[i]+grass_scale[i]*50.0f,grass_cy[i]+grass_scale[i]*100.0f);
        glVertex2f(grass_cx[i]+grass_scale[i]*-50.0f, grass_cy[i]+grass_scale[i]*100.0f);
        glVertex2f(grass_cx[i]+grass_scale[i]*-100.0f, grass_cy[i]+grass_scale[i]*250.0f);
        glEnd();
    }
}

// Draw Background Layers (Sand, River, Sky) - MODIFIED Sky position
void drawBackground()
{
    // Sand
    if (isNightMode) glColor3f(0.75f, 0.68f, 0.45f); // Slightly darker, less yellow sand
    else glColor3f(0.94f, 0.86f, 0.51f);
    glBegin(GL_POLYGON);
    glVertex2f(0,0);
    glVertex2f(1200,0);
    glVertex2f(1200,WATER_LEVEL_Y);
    glVertex2f(0,WATER_LEVEL_Y);
    glEnd();

    // River
    if (isNightMode) glColor3f(0.08f, 0.18f, 0.35f); // Darker blue/purple river
    else glColor3f(0.2f, 0.4f, 0.8f);
    // River goes up to Y=850
    glBegin(GL_POLYGON);
    glVertex2f(0,WATER_LEVEL_Y);
    glVertex2f(1200,WATER_LEVEL_Y);
    glVertex2f(1200,850);
    glVertex2f(0,850);
    glEnd();

    // Sky
    if (isNightMode) glColor3f(0.05f, 0.05f, 0.20f); // Dark deep blue sky
    else glColor3f(0.5f, 0.8f, 1.0f);
    // MODIFIED: Start sky slightly above river top edge (851 instead of 850)
    glBegin(GL_POLYGON);
    glVertex2f(0,851);
    glVertex2f(1200,851);
    glVertex2f(1200,1000);
    glVertex2f(0,1000);
    glEnd();
}

// Draw Colored Circle (Generic Helper)
void drawColoredCircle(GLfloat rx, GLfloat ry, GLfloat cx, GLfloat cy, GLfloat f, GLfloat l, GLfloat r, GLfloat g, GLfloat b)
{
    glColor3f(r,g,b);
    glBegin(GL_POLYGON);
    if(f==0 && l==360)
    {
        glVertex2f(cx,cy);
    }
    for(int i=f; i<=l; i++)
    {
        float a=i*3.1415926f/180.0f;
        float x=rx*cosf(a);
        float y=ry*sinf(a);
        glVertex2f(x+cx, y+cy);
    }
    glEnd();
}

// Draw Shadow Ellipse (Uses current color, expects glColor4f)
void drawShadowEllipse(GLfloat rx, GLfloat ry, GLfloat cx, GLfloat cy)
{
    // Assumes glColor4fv(shadowColor) called before this
    glBegin(GL_POLYGON);
    for(int i=0; i<=360; i+=10)   // Draw ellipse segments
    {
        float angle = i * 3.1415926f / 180.0f;
        float x = rx * cosf(angle);
        float y = ry * sinf(angle);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// Draw Sand House - Refined night colors
void drawSandHouse()
{
    GLfloat mainR = 0.6f,mainG = 0.3f,mainB = 0.1f;
    GLfloat detailR = 0.6f,detailG = 0.7f,detailB = 0.1f;
    GLfloat flagR = 0.9f,flagG = 0.1f,flagB = 0.1f;
    GLfloat detailsDarkR = 0.0f,detailsDarkG = 0.0f,detailsDarkB = 0.0f;
    if (isNightMode)
    {
        mainR=0.4f;
        mainG=0.2f;
        mainB=0.07f;
        detailR=0.45f;
        detailG=0.5f;
        detailB=0.1f;
        flagR=0.6f;
        flagG=0.05f;
        flagB=0.05f;
    }
    glPushMatrix();
    glTranslatef(sandhouse_X, sandhouse_Y, 0);
    glScalef(sandhouse_scale, sandhouse_scale, 1.0f);
    glBegin(GL_QUADS);
    glColor3f(mainR,mainG,mainB);
    glVertex2f(200, 50);
    glVertex2f(400, 50);
    glVertex2f(400, 250);
    glVertex2f(200, 250);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(270, 50);
    glVertex2f(330, 50);
    glVertex2f(330, 150);
    glVertex2f(270, 150);
    glEnd();
    drawColoredCircle(30,30, 300,150, 0,180, detailsDarkR,detailsDarkG,detailsDarkB);
    drawColoredCircle(10,20, 250,200, 0,360, detailsDarkR,detailsDarkG,detailsDarkB);
    drawColoredCircle(10,20, 350,200, 0,360, detailsDarkR,detailsDarkG,detailsDarkB);
    glColor3f(detailR,detailG,detailB);
    glBegin(GL_QUADS);
    glVertex2f(200, 250);
    glVertex2f(250, 250);
    glVertex2f(250, 300);
    glVertex2f(200, 300);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(280, 250);
    glVertex2f(320, 250);
    glVertex2f(320, 300);
    glVertex2f(280, 300);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(350, 250);
    glVertex2f(400, 250);
    glVertex2f(400, 300);
    glVertex2f(350, 300);
    glEnd();
    glColor3f(detailR,detailG,detailB);
    glBegin(GL_QUADS);
    glVertex2f(50, 50);
    glVertex2f(200, 50);
    glVertex2f(200, 150);
    glVertex2f(50, 150);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(50, 150);
    glVertex2f(100, 150);
    glVertex2f(100, 200);
    glVertex2f(50, 200);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(150, 150);
    glVertex2f(200, 150);
    glVertex2f(200, 200);
    glVertex2f(150, 200);
    glEnd();
    drawColoredCircle(10,20, 80,100, 0,360, detailsDarkR,detailsDarkG,detailsDarkB);
    drawColoredCircle(30,20, 150,100, 0,360, detailsDarkR,detailsDarkG,detailsDarkB);
    glColor3f(detailR,detailG,detailB);
    glBegin(GL_QUADS);
    glVertex2f(400, 50);
    glVertex2f(550, 50);
    glVertex2f(550, 200);
    glVertex2f(400, 200);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(500, 200);
    glVertex2f(550, 200);
    glVertex2f(550, 230);
    glVertex2f(500, 230);
    glEnd();
    drawColoredCircle(10,20, 440,150, 0,360, detailsDarkR,detailsDarkG,detailsDarkB);
    drawColoredCircle(10,20, 510,110, 0,360, detailsDarkR,detailsDarkG,detailsDarkB);
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(300, 300);
    glVertex2f(310, 300);
    glVertex2f(310, 380);
    glVertex2f(300, 380);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(flagR,flagG,flagB);
    glVertex2f(310, 350);
    glVertex2f(400, 365);
    glVertex2f(310, 380);
    glEnd();
    glPopMatrix();
}

// Draw Circle for Ball
void drawCircle(float rx, float ry, const Ball& ball, GLfloat f, GLfloat l)
{
    float r=ball.r, g=ball.g, b=ball.b;
    if (isNightMode)
    {
        r*=0.7f;
        g*=0.7f;
        b*=0.7f;
    }
    glColor3f(r,g,b);
    glBegin(GL_POLYGON);
    float cx=ball.x;
    float cy=ball.y;
    glVertex2f(cx,cy);
    for(int i=f; i<=l; i++)
    {
        float a=i*3.1415926f/180.0f;
        float x=rx*ball.scale*cosf(a);
        float y=ry*ball.scale*sinf(a);
        glVertex2f(x+cx, y+cy);
    }
    glEnd();
}
// Draw Red Triangle for Ball
void drawRedTriangle(const Ball& ball, float x1, float y1, float x2, float y2, float x3, float y3)
{
    if (isNightMode) glColor3f(0.6f,0.0f,0.0f);
    else glColor3f(1.0f,0.0f,0.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(ball.x+(x1-50)*ball.scale, ball.y+(y1-50)*ball.scale);
    glVertex2f(ball.x+(x2-50)*ball.scale, ball.y+(y2-50)*ball.scale);
    glVertex2f(ball.x+(x3-50)*ball.scale, ball.y+(y3-50)*ball.scale);
    glEnd();
}

// Draw Ball with Triangles
void drawBallWithTriangles(const Ball& ball)
{
    drawCircle(50,50,ball,0,360);
    drawRedTriangle(ball,40,0,60,0,50,50);
    drawRedTriangle(ball,50,50,97,30,97,70);
    drawRedTriangle(ball,0,40,50,50,0,60);
    drawRedTriangle(ball,50,50,60,100,40,100);
}

// Draw Text
void drawText(const char* text, float x, float y, void* font)
{
    if (isNightMode) glColor3f(0.8f, 0.8f, 0.8f);
    else glColor3f(0.0,0.0,0.0); // Lighter grey text at night
    int len=strlen(text);
    float tw=0;
    if(font==GLUT_BITMAP_HELVETICA_10)tw=len*6.0f;
    else if(font==GLUT_BITMAP_HELVETICA_12)tw=len*7.0f;
    else if(font==GLUT_BITMAP_HELVETICA_18)tw=len*10.0f;
    else tw=len*5.0f;
    glRasterPos2f(x-tw/2.0f, y);
    for(const char* c=text; *c!='\0'; c++)
    {
        glutBitmapCharacter(font,*c);
    }
}

// Draw Signboard
void drawSignboard()
{
    GLfloat poleR = 0.5f, poleG = 0.35f, poleB = 0.05f;
    GLfloat signR = 0.7f, signG = 0.5f, signB = 0.15f;
    GLfloat borderR = 0.0f, borderG = 0.0f, borderB = 0.0f;
    if (isNightMode)
    {
        poleR = 0.35f;
        poleG = 0.2f;
        poleB = 0.03f;
        signR = 0.5f;
        signG = 0.35f;
        signB = 0.1f;
        borderR = 0.1f;
        borderG = 0.1f;
        borderB = 0.1f;
    }
    glPushMatrix();
    glTranslatef(signboard_X,signboard_Y,0);
    glScalef(signboard_scale,signboard_scale,1.0f);
    glBegin(GL_QUADS);
    glColor3f(poleR,poleG,poleB);
    glVertex2f(-5,0);
    glVertex2f(5,0);
    glVertex2f(5,230);
    glVertex2f(-5,230);
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(signR,signG,signB);
    glVertex2f(-70,70);
    glVertex2f(60,70);
    glVertex2f(50,100);
    glVertex2f(60,130);
    glVertex2f(-70,130);
    glEnd();
    glColor3f(borderR, borderG, borderB);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-70,70);
    glVertex2f(60,70);
    glVertex2f(50,100);
    glVertex2f(60,130);
    glVertex2f(-70,130);
    glEnd();
    glLineWidth(1.0);
    glBegin(GL_POLYGON);
    glColor3f(signR,signG,signB);
    glVertex2f(-30,150);
    glVertex2f(70,150);
    glVertex2f(50,180);
    glVertex2f(70,200);
    glVertex2f(-30,200);
    glVertex2f(-80,170);
    glEnd();
    glColor3f(borderR, borderG, borderB);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(-30,150);
    glVertex2f(70,150);
    glVertex2f(50,180);
    glVertex2f(70,200);
    glVertex2f(-30,200);
    glVertex2f(-80,170);
    glEnd();
    glLineWidth(1.0);
    drawText(signboard_upperText,0,185,GLUT_BITMAP_HELVETICA_18);
    drawText(signboard_lowerText1,0,110,GLUT_BITMAP_HELVETICA_12);
    drawText(signboard_lowerText2,0,95,GLUT_BITMAP_HELVETICA_12);
    drawText(signboard_lowerText4,0,80,GLUT_BITMAP_HELVETICA_12);
    glPopMatrix();
}

// Draw Stars
void drawStars()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPointSize(1.5f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_STARS; ++i)
    {
        glVertex2f(starPositions[i][0], starPositions[i][1]);
    }
    glEnd();
    glPointSize(1.0f);
}
// Draw the Sun or Moon
void drawSunOrMoon()
{
    if (isNightMode)
    {
        GLfloat moonRadius = sun_baseRadius * 0.9f;
        GLfloat moon_color_r = 0.9f, moon_color_g = 0.9f, moon_color_b = 0.85f; // Off-white
        drawColoredCircle(moonRadius, moonRadius, sun_X, sun_Y, 0, 360, moon_color_r, moon_color_g, moon_color_b);
        drawStars(); // Draw stars only at night
    }
    else
    {
        sun_currentRadius = sun_baseRadius * sun_pulseScale;
        float haloRadius = sun_currentRadius * 1.15f;
        // Halo
        drawColoredCircle(haloRadius, haloRadius, sun_X, sun_Y, 0, 360, 1.0f, 0.95f, 0.4f);
        // Sun body
        drawColoredCircle(sun_currentRadius, sun_currentRadius, sun_X, sun_Y, 0, 360, sun_color_r, sun_color_g, sun_color_b);
    }
}

// Draw Tortoise - Uses controllable Y, checks visibility
void drawTortoise()
{
    if (!isTortoiseVisible) return; // Don't draw if vanished

    // Refined Night Colors
    GLfloat legR=0.0, legG=0.3, legB=0.0;
    GLfloat shellBodyR=0.1, shellBodyG=0.6, shellBodyB=0.1;
    GLfloat shellPatR=0.0, shellPatG=0.4, shellPatB=0.0;
    GLfloat headR=0.0, headG=0.3, headB=0.0;
    GLfloat eyeR=1.0, eyeG=1.0, eyeB=1.0;
    if (isNightMode)
    {
        legR=0.0;
        legG=0.18;
        legB=0.02;
        shellBodyR=0.05;
        shellBodyG=0.35;
        shellBodyB=0.05;
        shellPatR=0.0;
        shellPatG=0.25;
        shellPatB=0.02;
        headR=0.0;
        headG=0.18;
        headB=0.02;
        eyeR=0.8;
        eyeG=0.8;
        eyeB=0.8;
    }

    glPushMatrix();
    glTranslatef(tortoise_X, tortoise_Y, 0);
    glScalef(tortoise_scale, tortoise_scale, 1.0f);
    glColor3f(legR,legG,legB); // Legs
    glBegin(GL_QUADS);
    glVertex2f(-40,-10);
    glVertex2f(-20,-10);
    glVertex2f(-25,20);
    glVertex2f(-45,20);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(20,-10);
    glVertex2f(40,-10);
    glVertex2f(45,20);
    glVertex2f(25,20);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(25,50);
    glVertex2f(45,50);
    glVertex2f(40,80);
    glVertex2f(20,80);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(-45,50);
    glVertex2f(-25,50);
    glVertex2f(-20,80);
    glVertex2f(-40,80);
    glEnd();
    glColor3f(shellPatR,shellPatG,shellPatB);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glVertex2f(0,0);
    glVertex2f(0,60);
    glVertex2f(-25,5);
    glVertex2f(-25,55);
    glVertex2f(25,5);
    glVertex2f(25,55);
    glVertex2f(-45,30);
    glVertex2f(0,60);
    glVertex2f(45,30);
    glVertex2f(-45,30);
    glVertex2f(0,0);
    glVertex2f(45,30);
    glVertex2f(-25,5);
    glVertex2f(0,30);
    glVertex2f(25,5);
    glVertex2f(-25,55);
    glVertex2f(0,30);
    glVertex2f(25,55);
    glEnd();
    glLineWidth(1.0); // Shell Pattern
    drawColoredCircle(50,40,0,30,0,360, shellBodyR,shellBodyG,shellBodyB); // Shell Body
    drawColoredCircle(15,15,0,80,0,360, headR,headG,headB);
    drawColoredCircle(2,2,-5,85,0,360, eyeR,eyeG,eyeB);
    drawColoredCircle(2,2,5,85,0,360, eyeR,eyeG,eyeB); // Head & Eyes
    glPopMatrix();
}

// Draw Surfboard - Refined night colors
void drawSurfboard()
{
    GLfloat c1r=0.0, c1g=0.5, c1b=0.0;
    GLfloat c2r=0.4, c2g=0.8, c2b=0.3;
    GLfloat c3r=0.6, c3g=0.2, c3b=0.1;
    GLfloat c4r=0.0, c4g=0.0, c4b=0.1;
    if (isNightMode)
    {
        c1r=0.0;
        c1g=0.3;
        c1b=0.05;
        c2r=0.25;
        c2g=0.5;
        c2b=0.2;
        c3r=0.4;
        c3g=0.15;
        c3b=0.05;
        c4r=0.05;
        c4g=0.05;
        c4b=0.05;
    }
    glPushMatrix();
    glTranslatef(surfboard_X, surfboard_Y, 0);
    glScalef(surfboard_scale, surfboard_scale, 1.0f);
    glColor3f(c1r,c1g,c1b);
    glBegin(GL_TRIANGLES);
    glVertex2f(100,100);
    glVertex2f(150,50);
    glVertex2f(150,150);
    glEnd();
    drawColoredCircle(50,100,100,150,0,360, c2r,c2g,c2b);
    drawColoredCircle(50,100,100,150,90,270, c3r,c3g,c3b);
    drawColoredCircle(40,25,50,50,0,180, c4r,c4g,c4b);
    drawColoredCircle(30,18,110,50,0,180, c4r,c4g,c4b);
    glPopMatrix();
}

// Draw Handle - Refined night colors
void drawHandle()
{
    GLfloat c1r=0.0, c1g=0.5, c1b=0.0;
    GLfloat c2r=0.0, c2g=0.0, c2b=0.1;
    if (isNightMode)
    {
        c1r=0.0;
        c1g=0.3;
        c1b=0.05;
        c2r=0.05;
        c2g=0.05;
        c2b=0.05;
    }
    glPushMatrix();
    glTranslatef(handle_X, handle_Y, 0);
    glScalef(handle_scale, handle_scale, 1.0f);
    glColor3f(c1r,c1g,c1b);
    glBegin(GL_QUADS);
    glVertex2f(100,50);
    glVertex2f(110,50);
    glVertex2f(130,100);
    glVertex2f(110,100);
    glEnd();
    glBegin(GL_QUADS);
    glVertex2f(70,40);
    glVertex2f(120,40);
    glVertex2f(120,50);
    glVertex2f(80,50);
    glEnd();
    drawColoredCircle(20,30,80,20,0,180, c2r,c2g,c2b);
    drawColoredCircle(14,20,105,30,0,180, c2r,c2g,c2b);
    glPopMatrix();
}

// Draw Boat (User Controlled) - Refined night colors
void drawBoat()
{
    GLfloat c1r=0.0, c1g=0.5, c1b=0.0;
    GLfloat c2r=0.1, c2g=0.2, c2b=0.0;
    GLfloat c3r=0.1, c3g=0.0, c3b=0.1;
    GLfloat c4r=0.1, c4g=0.5, c4b=0.4;
    GLfloat c5r=0.1, c5g=0.0, c5b=0.4;
    if (isNightMode)
    {
        c1r=0.0;
        c1g=0.3;
        c1b=0.05;
        c2r=0.05;
        c2g=0.1;
        c2b=0.0;
        c3r=0.15;
        c3g=0.05;
        c3b=0.05;
        c4r=0.1;
        c4g=0.3;
        c4b=0.25;
        c5r=0.05;
        c5g=0.0;
        c5b=0.25;
    }
    glPushMatrix();
    glTranslatef(boat_X, boat_Y, 0);
    glScalef(boat_scale, boat_scale, 1.0f);
    glColor3f(c1r,c1g,c1b);
    glBegin(GL_TRIANGLES);
    glVertex2f(200,50);
    glVertex2f(250,50);
    glVertex2f(250,100);
    glEnd();
    glColor3f(c1r,c1g,c1b);
    glBegin(GL_TRIANGLES);
    glVertex2f(130,50);
    glVertex2f(130,70);
    glVertex2f(90,90);
    glEnd();
    glColor3f(c2r,c2g,c2b);
    glBegin(GL_TRIANGLES);
    glVertex2f(130,70);
    glVertex2f(130,90);
    glVertex2f(90,90);
    glEnd();
    glColor3f(c3r,c3g,c3b);
    glBegin(GL_QUADS);
    glVertex2f(130,50);
    glVertex2f(200,50);
    glVertex2f(220,70);
    glVertex2f(130,70);
    glEnd();
    glColor3f(c4r,c4g,c4b);
    glBegin(GL_QUADS);
    glVertex2f(130,70);
    glVertex2f(220,70);
    glVertex2f(250,100);
    glVertex2f(130,90);
    glEnd();
    glColor3f(c5r,c5g,c5b);
    glBegin(GL_QUADS);
    glVertex2f(150,70);
    glVertex2f(160,70);
    glVertex2f(160,90);
    glVertex2f(150,90);
    glEnd();
    glColor3f(c5r,c5g,c5b);
    glBegin(GL_QUADS);
    glVertex2f(180,70);
    glVertex2f(190,70);
    glVertex2f(190,93);
    glVertex2f(180,93);
    glEnd();
    glColor3f(c5r,c5g,c5b);
    glBegin(GL_QUADS);
    glVertex2f(210,70);
    glVertex2f(220,70);
    glVertex2f(220,95);
    glVertex2f(210,95);
    glEnd();
    glPopMatrix();
}

// Draw Stand (Watch Tower) - MODIFIED colors
void drawStand()
{
    GLfloat mainR=0.0, mainG=0.0, mainB=0.0; // Main body (black) - unchanged
    // MODIFIED: Detail color changed from green to brown
    GLfloat detailR=0.55f, detailG=0.27f, detailB=0.07f; // Wooden brown color for details/supports

    // MODIFIED: Night mode detail color changed to darker brown
    if (isNightMode)
    {
        detailR=0.35f;
        detailG=0.18f;
        detailB=0.05f;
    }

    glPushMatrix();
    glTranslatef(stand_X, stand_Y, 0.0f);
    glScalef(stand_scale, stand_scale, 1.0f);

    // Draw using the new detailR, detailG, detailB for parts that were green
    glBegin(GL_QUADS);
    glColor3f(mainR,mainG,mainB);
    glVertex2f(200,200);
    glVertex2f(350,200);
    glVertex2f(350,350);
    glVertex2f(200,350);
    glEnd(); // Main body

    // Layers - Use detail color
    for(int i=0; i<4; i++)
    {
        glBegin(GL_QUADS);
        glColor3f(detailR,detailG,detailB);
        float y=300-i*30;
        glVertex2f(200,y);
        glVertex2f(350,y);
        glVertex2f(350,y+20);
        glVertex2f(200,y+20);
        glEnd();
    }
// Vertical Stands, Crosses, Stairs, Supports, Roof Supports - Use detail color
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(200,50);
    glVertex2f(220,50);
    glVertex2f(220,200);
    glVertex2f(200,200);
    glEnd(); // Vert Stand L
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(330,50);
    glVertex2f(350,50);
    glVertex2f(350,200);
    glVertex2f(330,200);
    glEnd(); // Vert Stand R
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(220,130);
    glVertex2f(330,180);
    glVertex2f(330,200);
    glVertex2f(220,150);
    glEnd(); // Cross 1
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(220,180);
    glVertex2f(330,130);
    glVertex2f(330,150);
    glVertex2f(220,200);
    glEnd(); // Cross 2
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(50,50);
    glVertex2f(70,50);
    glVertex2f(200,160);
    glVertex2f(200,180);
    glEnd(); // Stairs
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(30,140);
    glVertex2f(50,140);
    glVertex2f(200,230);
    glVertex2f(200,250);
    glEnd(); // Stair holder
    float supports[][4]= {{60,60,70,150},{90,80,100,170},{130,120,140,190},{170,150,180,220}};
    for(int i=0; i<4; i++)
    {
        glBegin(GL_QUADS);    // Mid Supports
        glColor3f(detailR,detailG,detailB);
        glVertex2f(supports[i][0],supports[i][1]);
        glVertex2f(supports[i][2],supports[i][1]);
        glVertex2f(supports[i][2],supports[i][3]);
        glVertex2f(supports[i][0],supports[i][3]);
        glEnd();
    }
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(220,350);
    glVertex2f(230,350);
    glVertex2f(230,450);
    glVertex2f(220,450);
    glEnd(); // Roof Support L
    glBegin(GL_QUADS);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(330,350);
    glVertex2f(340,350);
    glVertex2f(340,450);
    glVertex2f(330,450);
    glEnd(); // Roof Support R

    // Roof - Use detail color
    glBegin(GL_TRIANGLES);
    glColor3f(detailR,detailG,detailB);
    glVertex2f(150,450);
    glVertex2f(400,450);
    glVertex2f(275,550);
    glEnd(); // Roof

    glPopMatrix();
}
// Draw Beach Seat - Refined night colors
void drawBeachSeat()
{
    GLfloat legR=0.5, legG=0.35, legB=0.05;
    GLfloat seatR=0.7, seatG=0.5, seatB=0.15;
    GLfloat backR=0.9, backG=0.8, backB=0.6;
    if (isNightMode)
    {
        legR=0.35f;
        legG=0.22f;
        legB=0.08f;
        seatR=0.5f;
        seatG=0.35f;
        seatB=0.1f;
        backR=0.7f;
        backG=0.6f;
        backB=0.45f;
    }
    glPushMatrix();
    glTranslatef(beachSeat_X, beachSeat_Y, 0);
    glScalef(beachSeat_scale, beachSeat_scale, 1.0f);
    glBegin(GL_QUADS);
    glColor3f(legR,legG,legB);
    glVertex2f(120,50);
    glVertex2f(130,50);
    glVertex2f(150,90);
    glVertex2f(140,90);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(legR,legG,legB);
    glVertex2f(170,40);
    glVertex2f(180,40);
    glVertex2f(180,90);
    glVertex2f(170,90);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(legR,legG,legB);
    glVertex2f(270,40);
    glVertex2f(280,40);
    glVertex2f(280,90);
    glVertex2f(270,90);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(legR,legG,legB);
    glVertex2f(270,40);
    glVertex2f(340,40);
    glVertex2f(340,50);
    glVertex2f(270,50);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(legR,legG,legB);
    glVertex2f(330,50);
    glVertex2f(340,50);
    glVertex2f(320,90);
    glVertex2f(310,90);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(seatR,seatG,seatB);
    glVertex2f(140,90);
    glVertex2f(320,90);
    glVertex2f(320,100);
    glVertex2f(140,100);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(legR,legG,legB);
    glVertex2f(140,100);
    glVertex2f(150,100);
    glVertex2f(60,200);
    glVertex2f(50,200);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(legR,legG,legB);
    glVertex2f(200,100);
    glVertex2f(210,100);
    glVertex2f(130,200);
    glVertex2f(140,200);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(backR,backG,backB);
    glVertex2f(150,100);
    glVertex2f(200,100);
    glVertex2f(140,200);
    glVertex2f(60,200);
    glEnd();
    glPopMatrix();
}

// Draw Cloud Component Circle - Refined night color
void drawCloudComponentCircle(GLfloat rx, GLfloat ry, GLfloat cx, GLfloat cy)
{
    if (isNightMode) glColor3f(0.65f, 0.65f, 0.75f); // Slightly bluish grey clouds
    else glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 360; i+=10)
    {
        float angle = i * 3.1415926f / 180.0f;
        float x = rx * cosf(angle);
        float y = ry * sinf(angle);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// Draw Clouds
void drawClouds()
{
    GLfloat baseColorR = 1.0, baseColorG = 1.0, baseColorB = 1.0;
    if (isNightMode)
    {
        baseColorR=0.65;
        baseColorG=0.65;
        baseColorB=0.75;
    }
    glPushMatrix();
    glTranslatef(cloud_X, cloud_Y, 0.0f);
    glScalef(cloud_scale, cloud_scale, 1.0f);
    drawCloudComponentCircle(50, 50, 200, 900);
    drawCloudComponentCircle(40, 40, 140, 875);
    drawCloudComponentCircle(30, 30, 100, 865);
    drawCloudComponentCircle(30, 30, 260, 880);
    glBegin(GL_QUADS);
    glColor3f(baseColorR, baseColorG, baseColorB);
    glVertex2f(100, 865);
    glVertex2f(260, 865);
    glVertex2f(260, 900);
    glVertex2f(100, 900);
    glEnd();
    drawCloudComponentCircle(45, 45, 200+450, 900);
    drawCloudComponentCircle(35, 35, 140+460, 875);
    drawCloudComponentCircle(28, 28, 260+445, 880);
    glBegin(GL_QUADS);
    glColor3f(baseColorR, baseColorG, baseColorB);
    glVertex2f(140+460, 875);
    glVertex2f(260+445, 875);
    glVertex2f(260+445, 900);
    glVertex2f(140+460, 900);
    glEnd();
    drawCloudComponentCircle(50, 50, 200+780, 900+20);
    drawCloudComponentCircle(40, 40, 140+780, 875+20);
    drawCloudComponentCircle(30, 30, 100+780, 865+20);
    drawCloudComponentCircle(30, 30, 260+780, 880+20);
    glBegin(GL_QUADS);
    glColor3f(baseColorR, baseColorG, baseColorB);
    glVertex2f(100+780, 865+20);
    glVertex2f(260+780, 865+20);
    glVertex2f(260+780, 900+20);
    glVertex2f(100+780, 900+20);
    glEnd();
    glPopMatrix();
}
// Draw Umbrella Base Circle - Refined night color
void drawUmbrellaBaseCircle(GLfloat rx, GLfloat ry, GLfloat cx, GLfloat cy)
{
    if (isNightMode) glColor3f(0.5f, 0.5f, 0.1f); // Darker yellow/brown base
    else glColor3f(0.8, 0.8, 0.0);
    glBegin(GL_POLYGON);
    glVertex2f(cx, cy);
    for (int i = 180; i <= 360; i++)
    {
        float angle = i * 3.1415926f / 180.0f;
        float x = rx * cosf(angle);
        float y = ry * sinf(angle);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}

// Draw Umbrella - Includes Shadow
void drawUmbrella()
{
    GLfloat poleR=0.5, poleG=0.35, poleB=0.05;
    GLfloat c1r=1.0, c1g=0.0, c1b=0.0;
    GLfloat c2r=0.0, c2g=1.0, c2b=0.0;
    GLfloat c3r=0.0, c3g=0.0, c3b=1.0;
    GLfloat c4r=1.0, c4g=1.0, c4b=0.0;
    GLfloat c5r=1.0, c5g=0.5, c5b=0.0;
    GLfloat c6r=0.5, c6g=0.0, c6b=1.0;
    if (isNightMode)
    {
        poleR=0.35f;
        poleG=0.2f;
        poleB=0.03f;
        c1r=0.7;
        c1g=0.0;
        c1b=0.0;
        c2r=0.0;
        c2g=0.7;
        c2b=0.0;
        c3r=0.0;
        c3g=0.0;
        c3b=0.7;
        c4r=0.7;
        c4g=0.7;
        c4b=0.0;
        c5r=0.7;
        c5g=0.35;
        c5b=0.0;
        c6r=0.35;
        c6g=0.0;
        c6b=0.7;
    }

    glPushMatrix();
    glTranslatef(umbrella_X, umbrella_Y, 0);
    glScalef(umbrella_scale, umbrella_scale, 1.0f);

    // Draw Shadow first (relative to umbrella position)
    glColor4fv(shadowColor); // Use the semi-transparent shadow color
    // Shadow position slightly offset from pole base (Y=-210)
    drawShadowEllipse(150, 40, 0, -200); // Wide, flat ellipse shadow for the canopy

    // Draw Pole
    glBegin(GL_QUADS);
    glColor3f(poleR,poleG,poleB);
    glVertex2f(-5,-210);
    glVertex2f(5,-210);
    glVertex2f(5,140);
    glVertex2f(-5,140);
    glEnd();
    // Draw Canopy Segments
    glBegin(GL_TRIANGLES);
    glColor3f(c1r,c1g,c1b);
    glVertex2f(0,145);
    glVertex2f(0,0);
    glVertex2f(80,0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(c2r,c2g,c2b);
    glVertex2f(0,145);
    glVertex2f(80,0);
    glVertex2f(130,0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(c3r,c3g,c3b);
    glVertex2f(0,145);
    glVertex2f(130,0);
    glVertex2f(180,0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(c4r,c4g,c4b);
    glVertex2f(0,145);
    glVertex2f(-80,0);
    glVertex2f(0,0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(c5r,c5g,c5b);
    glVertex2f(0,145);
    glVertex2f(-130,0);
    glVertex2f(-80,0);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(c6r,c6g,c6b);
    glVertex2f(0,145);
    glVertex2f(-180,0);
    glVertex2f(-130,0);
    glEnd();
    // Draw Base Circles
    drawUmbrellaBaseCircle(25,10,-155,0);
    drawUmbrellaBaseCircle(25,10,-105,0);
    drawUmbrellaBaseCircle(40,10,-40,0);
    drawUmbrellaBaseCircle(40,10,40,0);
    drawUmbrellaBaseCircle(25,10,105,0);
    drawUmbrellaBaseCircle(25,10,155,0);
    glPopMatrix();
}

// Draw Circle for Moving Boat - Refined night color
void drawMovingBoatCircle(GLfloat rx, GLfloat ry, GLfloat cx, GLfloat cy)
{
    if (isNightMode) glColor3f(0.75f, 0.75f, 0.75f); // Slightly brighter grey circles
    else glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POLYGON);
    glVertex2f(cx, cy);
    for (int i = 0; i <= 360; i+=20)
    {
        float angle = i * 3.1415926f / 180.0f;
        float x = rx * cosf(angle);
        float y = ry * sinf(angle);
        glVertex2f(x + cx, y + cy);
    }
    glEnd();
}
// Draw Moving Boat (Automatically moving) - Refined night colors
void drawMovingBoat()
{
    GLfloat c1r=0.0, c1g=0.0, c1b=0.0;
    GLfloat c2r=0.0, c2g=1.0, c2b=0.0;
    GLfloat c3r=1.0, c3g=0.0, c3b=0.0;
    GLfloat c4r=1.0, c4g=0.0, c4b=1.0;
    GLfloat c5r=1.0, c5g=0.5, c5b=1.0;
    GLfloat c6r=0.5, c6g=0.5, c6b=1.0;
    GLfloat c7r=0.8, c7g=0.0, c7b=0.004;
    if (isNightMode)
    {
        c2r=0.0;    // Darker, muted colors
        c2g=0.6;
        c2b=0.0;
        c3r=0.6;
        c3g=0.0;
        c3b=0.0;
        c4r=0.6;
        c4g=0.0;
        c4b=0.6;
        c5r=0.6;
        c5g=0.3;
        c5b=0.6;
        c6r=0.3;
        c6g=0.3;
        c6b=0.6;
        c7r=0.5;
        c7g=0.0;
        c7b=0.0;
    }
    glPushMatrix();
    glTranslatef(movingBoat_X, movingBoat_Y, 0);
    glScalef(movingBoat_scale, movingBoat_scale, 1.0f);
    glBegin(GL_QUADS);
    glColor3f(c1r, c1g, c1b);
    glVertex2f(250, 250);
    glVertex2f(450, 250);
    glVertex2f(500, 300);
    glVertex2f(200, 300);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(c2r, c2g, c2b);
    glVertex2f(250, 250);
    glVertex2f(450, 250);
    glVertex2f(470, 270);
    glVertex2f(230, 270);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(c3r, c3g, c3b);
    glVertex2f(260, 240);
    glVertex2f(440, 240);
    glVertex2f(450, 250);
    glVertex2f(250, 250);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(c4r, c4g, c4b);
    glVertex2f(200, 300);
    glVertex2f(280, 300);
    glVertex2f(280, 330);
    glVertex2f(170, 330);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(c5r, c5g, c5b);
    glVertex2f(450, 300);
    glVertex2f(500, 300);
    glVertex2f(520, 320);
    glVertex2f(450, 320);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(c6r, c6g, c6b);
    glVertex2f(350, 300);
    glVertex2f(450, 300);
    glVertex2f(450, 320);
    glVertex2f(350, 320);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(c6r, c6g, c6b);
    glVertex2f(350, 320);
    glVertex2f(470, 320);
    glVertex2f(470, 350);
    glVertex2f(350, 350);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(c6r, c6g, c6b);
    glVertex2f(330, 350);
    glVertex2f(490, 350);
    glVertex2f(490, 370);
    glVertex2f(330, 370);
    glEnd();
    glBegin(GL_QUADS);
    glColor3f(c1r, c1g, c1b);
    glVertex2f(250, 330);
    glVertex2f(260, 330);
    glVertex2f(260, 430);
    glVertex2f(250, 430);
    glEnd();
    glBegin(GL_TRIANGLES);
    glColor3f(c7r, c7g, c7b);
    glVertex2f(260, 370);
    glVertex2f(360, 400);
    glVertex2f(260, 430);
    glEnd();
    drawMovingBoatCircle(10, 10, 250, 285);
    drawMovingBoatCircle(10, 10, 350, 285);
    drawMovingBoatCircle(10, 10, 450, 285);
    drawMovingBoatCircle(20, 20, 400, 330);
    glPopMatrix();
}

// Draw Coconut Circle - Refined night color
void drawCoconutCircle(float cx, float cy, float r, int segments)
{
    if (isNightMode) glColor3f(0.25f, 0.12f, 0.0f); // Darker brown
    else glColor3f(0.4f, 0.2f, 0.0f);
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; ++i)
    {
        float theta = 2.0f * 3.1415926f * float(i) / float(segments);
        float x = r * cosf(theta);
        float y = r * sinf(theta);
        glVertex2f(cx + x, cy + y);
    }
    glEnd();
}

// Draw a single tree - Includes Shadow
void drawSingleTree()
{
    GLfloat trunkR = 0.55f, trunkG = 0.27f, trunkB = 0.07f;
    GLfloat leavesR = 0.0f, leavesG = 0.5f, leavesB = 0.0f;
    if (isNightMode)
    {
        trunkR = 0.35f;    // Darker, less saturated
        trunkG = 0.18f;
        trunkB = 0.05f;
        leavesR = 0.0f;
        leavesG = 0.3f;
        leavesB = 0.05f;
    }

    // Draw Shadow first (relative to tree's base (0,0))
    glColor4fv(shadowColor);
    drawShadowEllipse(35, 15, 0, 5); // Shadow at the base of the trunk
// Draw Trunk
    glColor3f(trunkR, trunkG, trunkB);
    float trunk_base_y = 0;
    float trunk_segment_h = 30;
    float trunk_bottom_w = 5;
    float trunk_top_w = 10;
    float trunk_center_x = 0;
    for (int i = 0; i < 5; i++)
    {
        glBegin(GL_QUADS);
        glVertex2f(trunk_center_x - trunk_bottom_w, trunk_base_y + i * trunk_segment_h);
        glVertex2f(trunk_center_x + trunk_bottom_w, trunk_base_y + i * trunk_segment_h);
        glVertex2f(trunk_center_x + trunk_top_w,   trunk_base_y + (i + 1) * trunk_segment_h);
        glVertex2f(trunk_center_x - trunk_top_w,   trunk_base_y + (i + 1) * trunk_segment_h);
        glEnd();
    }
    float trunk_top_y = trunk_base_y + 5 * trunk_segment_h;

    // Draw Leaves
    glColor3f(leavesR, leavesG, leavesB);
    glBegin(GL_TRIANGLES);
    glVertex2f(trunk_center_x, trunk_top_y);
    glVertex2f(trunk_center_x - 70, trunk_top_y + 50);
    glVertex2f(trunk_center_x, trunk_top_y + 30);
    glVertex2f(trunk_center_x, trunk_top_y);
    glVertex2f(trunk_center_x + 70, trunk_top_y + 50);
    glVertex2f(trunk_center_x, trunk_top_y + 30);
    glVertex2f(trunk_center_x, trunk_top_y);
    glVertex2f(trunk_center_x - 80, trunk_top_y + 10);
    glVertex2f(trunk_center_x - 10, trunk_top_y - 10);
    glVertex2f(trunk_center_x, trunk_top_y);
    glVertex2f(trunk_center_x + 80, trunk_top_y + 10);
    glVertex2f(trunk_center_x + 10, trunk_top_y - 10);
    glVertex2f(trunk_center_x, trunk_top_y);
    glVertex2f(trunk_center_x, trunk_top_y + 80);
    glVertex2f(trunk_center_x + 20, trunk_top_y);
    glVertex2f(trunk_center_x, trunk_top_y);
    glVertex2f(trunk_center_x - 20, trunk_top_y);
    glVertex2f(trunk_center_x, trunk_top_y + 80);
    glEnd();

    // Draw Coconuts
    drawCoconutCircle(trunk_center_x - 5, trunk_top_y - 5, 5, 20);
    drawCoconutCircle(trunk_center_x + 5, trunk_top_y - 5, 5, 20);
    drawCoconutCircle(trunk_center_x, trunk_top_y - 15, 5, 20);
}

// Draw all trees - MODIFIED: Use master scale
void drawAllTrees()
{
    for (int i = 0; i < NUM_TREES; i++)
    {
        glPushMatrix();
        glTranslatef(tree_cx[i], tree_cy[i], 0.0f);
        // MODIFIED: Use the master scale for all trees
        glScalef(tree_master_scale, tree_master_scale, 1.0f);
        drawSingleTree(); // Draws tree and its shadow
        glPopMatrix();
    }
}

// Draw Waves - Refined night color
void drawWaves()
{
    float waterLineY = WATER_LEVEL_Y;
    if (isNightMode)
    {
        glColor3f(0.15f, 0.25f, 0.45f);    // Darker blue waves
    }
    else
    {
        glColor3f(0.3f, 0.5f, 0.9f);
    }
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= waveSegments; ++i)
    {
        float x = (1200.0f / waveSegments) * i;
        float yOffset = waveAmplitude * sinf(x * waveFrequency + wavePhase);
        glVertex2f(x, waterLineY + yOffset);
        glVertex2f(x, waterLineY - waveAmplitude * 1.5f);
    }
    glEnd();
    if (isNightMode) glColor3f(0.5f, 0.5f, 0.6f); // Greyer foam at night
    else glColor3f(1.0f, 1.0f, 1.0f);
    glLineWidth(1.5f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= waveSegments; ++i)
    {
        float x = (1200.0f / waveSegments) * i;
        float yOffset = waveAmplitude * sinf(x * waveFrequency + wavePhase);
        glVertex2f(x, waterLineY + yOffset);
    }
    glEnd();
    glLineWidth(1.0f);
}

// Draw Dolphin
void drawDolphin(const Dolphin& d)
{
    if (!d.visible) return;
    
    glPushMatrix();
    glTranslatef(d.x, d.y, 0.0f);
    glRotatef(d.angle, 0, 0, 1); // Rotate dolphin based on trajectory
    glScalef(d.scale, d.scale, 1.0f);
    
    // Body (Simple ellipse)
    GLfloat bodyR = 0.5f, bodyG = 0.55f, bodyB = 0.6f; // Greyish blue
    if (isNightMode)
    {
        bodyR=0.3f;
        bodyG=0.35f;
        bodyB=0.4f;
    }
    drawColoredCircle(50, 25, 0, 0, 0, 360, bodyR, bodyG, bodyB); // Main body ellipse
    
    // Tail fin (Triangle) - position relative to body center (0,0)
        glColor3f(bodyR * 0.8f, bodyG * 0.8f, bodyB * 0.8f); // Slightly darker fin
        glBegin(GL_TRIANGLES);
        glVertex2f(-45, 5);  // Point 1 (base left)
        glVertex2f(-45, -5); // Point 2 (base right)
        glVertex2f(-70, 0);  // Point 3 (tip)
        glEnd();

        // Dorsal fin (Triangle)
        glBegin(GL_TRIANGLES);
        glVertex2f(-10, 20); // Base back
        glVertex2f(10, 20);  // Base front
        glVertex2f(0, 40);   // Tip
        glEnd();

        // Pectoral fin (Triangle - simple side fin)
        glBegin(GL_TRIANGLES);
        glVertex2f(15, -15); // Base top
        glVertex2f(35, -20); // Tip
        glVertex2f(25, -25); // Base bottom
        glEnd();

        glPopMatrix();
    }


    // --- OpenGL Setup and Main Loop ---

    // Initialize OpenGL - MODIFIED: Dolphin init, removed tree scale array init
    void init(void)
    {
        // MODIFIED: Removed individual tree scale initialization

        // Set up projection
        glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Initial day clear color
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(0, 1200, 0, 1000);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        // Enable Alpha Blending for shadows
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Seed random number generator
        srand(time(0));

        // Initialize Star Positions (within sky area)
        for (int i = 0; i < NUM_STARS; ++i)
        {
            starPositions[i][0] = (float)(rand() % 1200);
            starPositions[i][1] = 851.0f + (float)(rand() % 149); // Positioned within the adjusted sky area (851-1000)
        }

        // MODIFIED: Initialize Dolphins (Smaller and further out)
        dolphins.clear(); // Clear existing dolphins if init is called again
        for (int i=0; i < NUM_DOLPHINS; ++i)
        {
            // MODIFIED: Place dolphins further out, avoiding near shore and boat area
            float startX = 400.0f + (float)(rand() % 600); // Random start X between 400 and 1000
            // MODIFIED: Make dolphins smaller
            float startScale = 0.25f + (float)(rand() % 2)/10.0f; // Random scale 0.25-0.35
            float startInterval = 70.0f + (rand() % 180); // Random initial interval
            dolphins.push_back(Dolphin(startX, WATER_LEVEL_Y, startScale, startInterval));
        }
    }

    // Animation Update Function
    void update(int value)
    {
        // Cloud Animation
        cloud_X -= 0.4f;
        if ( (cloud_X + (260.0f + 780.0f + 50.0f) * cloud_scale) < 0)
        {
            cloud_X = 1200;    // Adjusted check width slightly
        }
        // Moving Boat Animation
        movingBoat_X -= 1.0f;
        if ( (movingBoat_X + 520.0f * movingBoat_scale) < 0)
        {
            movingBoat_X = 1300;
        }
        // Sun Pulse Animation (Day Only)
        if (!isNightMode)
        {
            sun_pulseScale += sun_pulseDirection;
            if (sun_pulseScale > sun_maxScale ||  sun_pulseScale < sun_minScale)
            {
                sun_pulseDirection *= -1.0f;
                sun_pulseScale = (sun_pulseScale > sun_maxScale) ? sun_maxScale : sun_minScale;
            }
        }
        // Wave Animation
        wavePhase += 0.05f;

        // Update Dolphins
        for (int i = 0; i < dolphins.size(); ++i)
        {
            dolphins[i].update(1.0f); // Update each dolphin
        }

        glutPostRedisplay();
        glutTimerFunc(16, update, 0); // Aim for ~60 FPS
    }

    // Keyboard Input for Movable Boat & Tortoise
    void specialKeys(int key, int x, int y)
    {
        switch (key)
        {
        // Boat Movement
        case GLUT_KEY_LEFT:
            boat_X -= boat_move_speed;
            if (boat_X < boat_left_margin) boat_X = boat_left_margin;
            break;
        case GLUT_KEY_RIGHT:
            boat_X += boat_move_speed;
            if (boat_X > boat_right_margin) boat_X = boat_right_margin;
            break;
    // Tortoise Movement
        case GLUT_KEY_UP:
            if (isTortoiseVisible)
            {
                tortoise_Y += tortoise_move_speed;
                // Check for vanishing condition
                if (tortoise_Y > tortoise_Y_max_disappear)
                {
                    isTortoiseVisible = false;
                    // Optionally reset Y slightly below vanish point
                    // tortoise_Y = tortoise_Y_max_disappear - 1.0f;
                }
            }
            break;
        case GLUT_KEY_DOWN:
            if (isTortoiseVisible)
            {
                tortoise_Y -= tortoise_move_speed;
                // Clamp to minimum Y
                if (tortoise_Y < tortoise_Y_min)
                {
                    tortoise_Y = tortoise_Y_min;
                }
            }
            // Tortoise reappears on Day/Night toggle now.
            break;
        }
        glutPostRedisplay();
    }

    // Keyboard Input for Day/Night Toggle - Resets tortoise visibility
    void keyboardHandler(unsigned char key, int x, int y)
    {
        if (key == 'n' || key == 'N')
        {
            isNightMode = !isNightMode;
            isTortoiseVisible = true; // Reset tortoise visibility when mode changes
            // If tortoise vanished, reset its Y position so it's visible
            if (tortoise_Y > tortoise_Y_max_disappear)
            {
                tortoise_Y = tortoise_Y_max_disappear - tortoise_move_speed; // Place it just below vanish point
            }
            // Update clear color based on mode
            if (isNightMode)
            {
                glClearColor(0.05f, 0.05f, 0.20f, 1.0f); // Night sky color
            }
            else
            {
                glClearColor(0.5f, 0.8f, 1.0f, 1.0f); // Day sky color
            }
            glutPostRedisplay();
        }
        // Optional quit key
        // if (key == 'q'  key == 'Q') { exit(0); }
    }


    // Display callback - Draws the entire scene
    void display(void)
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();

        // Draw layers from back to front
        drawBackground();    // Sand, River, Sky
        drawWaves();         // Animated waves on the river
        drawSunOrMoon();     // Sun or Moon/Stars depending on mode
        drawClouds();        // Moving clouds

        // Draw elements in the water
        for (int i = 0; i < dolphins.size(); ++i)
        {
            drawDolphin(dolphins[i]); // Jumping dolphins
        }
        drawBoat();          // User-controlled boat
        drawMovingBoat();    // Automatically moving boat
    // Draw land/sand items (Shadows are drawn within these functions now)
        drawAllTrees();      // Draws trees and their shadows
        // Oysters
        int num_oysters = sizeof(oyster_scale)/sizeof(oyster_scale[0]);
        for(int i=0; i<num_oysters; i++)
        {
            GLfloat shell1R=0.95, shell1G=0.95, shell1B=0.9;
            GLfloat pearlR=0.98, pearlG=0.98, pearlB=0.98;
            GLfloat shell2R=0.90, shell2G=0.90, shell2B=0.85;
            GLfloat hingeR=0.7, hingeG=0.65, hingeB=0.6;
            if (isNightMode)
            {
                shell1R=0.75;
                shell1G=0.75;
                shell1B=0.7;
                pearlR=0.85;
                pearlG=0.85;
                pearlB=0.85;
                shell2R=0.7;
                shell2G=0.7;
                shell2B=0.65;
                hingeR=0.5;
                hingeG=0.45;
                hingeB=0.4;
            }
            glColor3f(shell1R,shell1G,shell1B);
            drawEllipseSector(90*oyster_scale[i],60*oyster_scale[i], oyster_cx[i],oyster_cy[i],0,180);
            glColor3f(pearlR,pearlG,pearlB);
            drawEllipseSector(15*oyster_scale[i],10*oyster_scale[i], oyster_cx[i]-75*oyster_scale[i],oyster_cy[i],0,360);
            drawEllipseSector(15*oyster_scale[i],10*oyster_scale[i], oyster_cx[i]-45*oyster_scale[i],oyster_cy[i],0,360);
            drawEllipseSector(15*oyster_scale[i],10*oyster_scale[i], oyster_cx[i]-15*oyster_scale[i],oyster_cy[i],0,360);
            drawEllipseSector(15*oyster_scale[i],10*oyster_scale[i], oyster_cx[i]+15*oyster_scale[i],oyster_cy[i],0,360);
            drawEllipseSector(15*oyster_scale[i],10*oyster_scale[i], oyster_cx[i]+45*oyster_scale[i],oyster_cy[i],0,360);
            drawEllipseSector(15*oyster_scale[i],10*oyster_scale[i], oyster_cx[i]+75*oyster_scale[i],oyster_cy[i],0,360);
            glColor3f(shell2R,shell2G,shell2B);
            drawEllipseSector(90*oyster_scale[i],60*oyster_scale[i], oyster_cx[i],oyster_cy[i],180,360);
            glColor3f(hingeR,hingeG,hingeB);
            drawEllipseSector(27*oyster_scale[i],20*oyster_scale[i], oyster_cx[i],oyster_cy[i]+58*oyster_scale[i],0,180);
        }
        // Starfish
        int num_starfish = sizeof(star_scale)/sizeof(star_scale[0]);
        for(int i=0; i<num_starfish; i++)
        {
            drawStarfish(star_scale[i], starcx[i], starcy[i]);
        }

        drawGrass();         // Patches of grass
        drawTortoise();      // Controllable tortoise
        drawSurfboard();     // Surfboard on the sand
        drawHandle();        // Handle (presumably for bucket)
        drawStand();         // Watch tower (now brown)
        drawBeachSeat();     // Beach chair
        drawUmbrella();      // Umbrella and its shadow
        drawSignboard();     // Signboard with text
        drawSandHouse();     // Sandcastle
        for (const auto & ball : balls)   // Beach balls
        {
            drawBallWithTriangles(ball);
        }

        glutSwapBuffers(); // Display the drawn buffer
    }


    // Main function
    int main(int argc, char** argv)
    {
        glutInit(&argc, argv);
        // MODIFIED: Added GLUT_ALPHA for transparency needed for shadows
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
        glutInitWindowSize(1200, 1000);
        glutInitWindowPosition(50, 50);
        // MODIFIED: Updated window title to reflect changes and location context
        glutCreateWindow("Beach Scene - Ashulia (Dolphin, Shadows, Tortoise Ctrl, Mods)");
        init(); // Initialize OpenGL state, objects, etc.

        // Register callbacks
        glutDisplayFunc(display);          // Function to call for drawing
        glutSpecialFunc(specialKeys);      // Function for arrow keys etc.
        glutKeyboardFunc(keyboardHandler);
        // Function for normal keys ('N')
        glutTimerFunc(0, update, 0);
        // Function for animation updates

        // Start the GLUT main loop
        glutMainLoop();
        return 0;
        // Unreachable in GLUT, but good practice
    }



