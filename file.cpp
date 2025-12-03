#include "iGraphics.h"

int gameState = 0;   // 0 = menu, 1 = play

int playerX = 100;
int playerY = 100;
int playerW = 40;
int playerH = 40;

int jumpY = 0;
int isJumping = 0;
int jumpUp = 1;

int obsX = 800;
int obsY = 100;
int obsW = 40;
int obsH = 40;

int gameOver = 0;

int moveTimer, jumpTimer;

void resetGame() {
    gameOver = 0;
    obsX = 800;
    jumpY = 0;
    isJumping = 0;
    jumpUp = 1;
}

void moveObstacle() {
    if(gameState != 1){
        return;
    }
    if(gameOver){
        return;
    }

    obsX -= 10;
    if(obsX + obsW < 0) {
        obsX = 800;
    }

    int px = playerX;
    int py = playerY + jumpY;

    int ox = obsX;
    int oy = obsY;

    if(px < ox + obsW && px + playerW > ox && py < oy + obsH && py + playerH > oy){
        gameOver = 1;
    }
}

void jumpUpdate() {
    if(gameState != 1){
        return;
    }
    if(!isJumping){
        return;
    }
    if(jumpUp){
        jumpY += 10;
        if(jumpY >= 120) jumpUp = 0;
    }
    else{
        jumpY -= 10;
        if(jumpY <= 0) {
            jumpY = 0;
            isJumping = 0;
            jumpUp = 1;
        }
    }
}

void iDraw() {
    iClear();
    // ---------------------
    // MENU SCREEN
    // ---------------------
    if(gameState == 0) {
        iSetColor(255, 255, 255);
        iText(300, 400, "BOX RUNNER", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(260, 340, "Press SPACE to Start", GLUT_BITMAP_HELVETICA_18);
        iText(250, 300, "Press ESC to Quit", GLUT_BITMAP_HELVETICA_18);
        return;
    }

    // ---------------------
    // GAME OVER SCREEN
    // ---------------------
    if(gameOver) {
        iSetColor(255, 0, 0);
        iText(350, 300, "GAME OVER", GLUT_BITMAP_HELVETICA_18);
        iText(300, 260, "Press R to Restart", GLUT_BITMAP_HELVETICA_18);
        return;
    }
    // ---------------------
    // GAMEPLAY SCREEN
    // ---------------------

    // Player
    iSetColor(0, 200, 255);
    iFilledRectangle(playerX, playerY + jumpY, playerW, playerH);

    // Obstacle
    iSetColor(255, 120, 0);
    iFilledRectangle(obsX, obsY, obsW, obsH);
}

void iKeyPress(unsigned char key)
{
    // MENU → PLAY
    if(gameState == 0) {
        if(key == ' ') {
            gameState = 1;
            return;
        }
        if(key == 27) exit(0);
    }

    // GAMEPLAY
    if(gameState == 1) {

        if(key == ' ') {
            if(!isJumping && !gameOver) {
                isJumping = 1;
                jumpUp = 1;
            }
        }
        if((key == 'r' || key == 'R') && gameOver) {
            resetGame();
        }
    }
}

void iSpecialKeyboard(unsigned char key) { }

int main() {
    moveTimer = iSetTimer(30, moveObstacle);
    jumpTimer = iSetTimer(30, jumpUpdate);
    iWindowedMode(800, 600, "Box Runner Game");
    iStartMainLoop();
    return 0;
}
