#include "iGraphics.h"

int playerX = 100;
int playerY = 100;
int playerW = 50;
int playerH = 50;

int obsX = 800;
int obsY = 100;
int obsW = 40;
int obsH = 40;

int jumping = 0;
int jumpSpeed = 0;
int gravity = 2;

int gameOver = 0;

void iDraw()
{
    iClear();

    if (gameOver)
    {
        iSetColor(255, 0, 0);
        iText(350, 300, "GAME OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(330, 260, "Press R to Restart", GLUT_BITMAP_TIMES_ROMAN_24);
        return;
    }

    // Player
    iSetColor(0, 255, 0);
    iFilledRectangle(playerX, playerY, playerW, playerH);

    // Obstacle
    iSetColor(255, 0, 0);
    iFilledRectangle(obsX, obsY, obsW, obsH);
}

void update()
{
    if (gameOver) return;

    // obstacle move
    obsX -= 5;
    if (obsX <= -50)
        obsX = 800;

    // jump physics
    if (jumping)
    {
        playerY += jumpSpeed;
        jumpSpeed -= gravity;

        if (playerY <= 100)
        {
            playerY = 100;
            jumping = 0;
        }
    }

    // collision detection
    int hitX = (playerX + playerW > obsX) && (playerX < obsX + obsW);
    int hitY = (playerY < obsY + obsH);

    if (hitX && hitY)
        gameOver = 1;
}

void iKeyboard(unsigned char key)
{
    if (key == ' ')
    {
        if (!jumping && !gameOver)
        {
            jumping = 1;
            jumpSpeed = 20;
        }
    }

    if (key == 'r')
    {
        gameOver = 0;
        obsX = 800;
        playerY = 100;
        jumping = 0;
    }
}

void iMouseMove(int mx, int my) {}
void iMouse(int button, int state, int mx, int my) {}
void iSpecialKeyboard(unsigned char key) {}

int main()
{
    iSetTimer(15, update);
    iWindowedMode(800, 600, "Runner Game");
    iStartMainLoop();
    return 0;
}
