#include "iGraphics.h"

int playerX = 100;         // Player স্থির পজিশন
int playerY = 100;         // Ground level
int playerW = 50;
int playerH = 50;

int obsX = 800;            // Obstacle শুরু পজিশন (ডানের দিক)
int obsY = 100;
int obsW = 40;
int obsH = 40;

int jump = 0;              // 0 = না, 1 = জাম্পিং
int jumpSpeed = 0;
int gravity = 2;

bool gameOver = false;

void iDraw()
{
    iClear();

    if (gameOver)
    {
        iText(300, 300, "Game Over!", GLUT_BITMAP_TIMES_ROMAN_24);
        return;
    }

    // Player Box
    iSetColor(0, 255, 0);
    iFilledRectangle(playerX, playerY, playerW, playerH);

    // Obstacle Box
    iSetColor(255, 0, 0);
    iFilledRectangle(obsX, obsY, obsW, obsH);
}

void updateGame()
{
    if (gameOver) return;

    // অবস্ট্যাকল বাম দিকে আসবে
    obsX -= 5;

    if (obsX < -50)
        obsX = 800;   // আবার ডানে পাঠানো হবে

    // জাম্প লজিক
    if (jump)
    {
        playerY += jumpSpeed;
        jumpSpeed -= gravity;

        if (playerY <= 100)
        {
            playerY = 100;
            jump = 0;
        }
    }

    // Collision Check
    bool collideX = (playerX + playerW > obsX) && (playerX < obsX + obsW);
    bool collideY = (playerY < obsY + obsH);

    if (collideX && collideY)
    {
        gameOver = true;
    }
}

void iKeyboard(unsigned char key)
{
    if (key == ' ')
    {
        if (!jump && !gameOver)
        {
            jump = 1;
            jumpSpeed = 20;   // Jump power
        }
    }

    if (key == 'r')
    {
        // Restart
        gameOver = false;
        obsX = 800;
        playerY = 100;
        jump = 0;
    }
}

void iSpecialKeyboard(unsigned char key) {}

int main()
{
    iSetTimer(15, updateGame);
    iInitialize(900, 500, "Basic Runner Game");
    return 0;
}
