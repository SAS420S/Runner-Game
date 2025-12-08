#include<stdio.h>
#include "iGraphics.h"
#include "iSound.h"

// Running frames
Image runFrames[10];
Image jumpFrames[10];
Image slideFrames[10];
Image deathFrames[10];
Image obstacleImg, bg;

// GAME VARIABLES
int gameState = 0;

int playerX = 150;
int playerY = 150;

int jumpY = 0;
int isJumping = 0;
int jumpUp = 1;

int isSliding = 0;

int playerFrame = 0;
int frameTimer;

int obsX = 1200;
int obsY = 150;
int obsW = 40;
int obsH = 40;

int obsType = 1;

int gameOver = 0;
int score = 0;

int moveTimer, jumpTimer;

//Animation frame every few milliseconds
void updateFrame() {
    if(gameState != 1) return;
    if(gameOver) {
        // Death animation runs slower
        if(playerFrame < 9) playerFrame++;
        return;
    }
    playerFrame++;
    if(playerFrame >= 10) playerFrame = 0;
}

void resetGame() {
    gameOver = 0;
    obsX = 1200;
    jumpY = 0;
    isJumping = 0;
    jumpUp = 1;
    isSliding = 0;
    playerFrame = 0;
}

void createNewObstacle() {
    obsX = 1200;
    if(rand() % 2 == 0) obsType = 1;
    else obsType = 2;
    if(obsType == 2) {
        obsH = 20;
        obsY = 150;
    }
    else {
        obsH = 40;
        obsY = 150;
    }
}

// MOVE OBSTACLE + COLLISION CHECK
void moveObstacle() {
    if(gameState != 1) return;
    if(gameOver) return;
    obsX -= 10;
    score += 1;
    if(obsX + obsW < 0) {
        createNewObstacle();
    }
    int px = playerX;
    int py = playerY + jumpY;
    int ph = isSliding ? 20 : 40;
    int ox = obsX;
    int oy = obsY;
    if(px < ox + obsW && px + 40 > ox && py < oy + obsH && py + ph > oy) {
        if(obsType == 2 && isSliding) {
            // SAFE
        }
        else {
            gameOver = 1;
            playerFrame = 0;  // Start death animation
        }
    }
}

void jumpUpdate() {
    if(gameState != 1) return;
    if(!isJumping) return;
    if(isSliding) return;
    if(jumpUp) {
        jumpY += 10;
        if(jumpY >= 120) jumpUp = 0;
    }
    else {
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

    iShowLoadedImage(0, 0, &bg);
    // MENU
    if(gameState == 0) {
        iText(300, 400, "ROAD RUNNER", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(260, 340, "Press SPACE to Start", GLUT_BITMAP_HELVETICA_18);
        return;
    }
    // GAME OVER
    if(gameOver) {
        iShowLoadedImage(playerX, playerY, &deathFrames[playerFrame]);
        iText(350, 300, "GAME OVER", GLUT_BITMAP_HELVETICA_18);
        iText(300, 260, "Press R to Restart", GLUT_BITMAP_HELVETICA_18);
        return;
    }
    // DRAW PLAYER SPRITES
    if(isJumping) {
        iShowLoadedImage(playerX, playerY + jumpY, &jumpFrames[playerFrame]);
    }
    else if(isSliding) {
        iShowLoadedImage(playerX, playerY, &slideFrames[playerFrame]);
    }
    else {
        iShowLoadedImage(playerX, playerY, &runFrames[playerFrame]);
    }
    iText(100, 700, "Score: ", GLUT_BITMAP_HELVETICA_18);
    // DRAW OBSTACLE
    iShowLoadedImage(obsX, obsY, &obstacleImg);
}

void iKeyPress(unsigned char key) {
    if(gameState == 0) {
        if(key == ' ') gameState = 1;
        return;
    }
    if(gameState == 1) {
        if(key == ' ') {
            if(!isJumping && !isSliding && !gameOver) {
                isJumping = 1;
                jumpUp = 1;
                playerFrame = 0;
            }
        }
        if(key == 's') {
            if(!isJumping && !gameOver) {
                isSliding = 1;
                playerFrame = 0;
            }
        }
        if((key == 'r' || key == 'R') && gameOver) {
            resetGame();
        }
    }
}

// RELEASE SLIDE KEY
void iKeyUp(unsigned char key) {
    if(key == 's') {
        isSliding = 0;
    }
}

void iSpecialKeyPress(unsigned char key) {}


void loadResources() {
    // Load player running frames
    iLoadImage(&runFrames[0], "assets/images/Run0.png");
    iLoadImage(&runFrames[1], "assets/images/Run1.png");
    iLoadImage(&runFrames[2], "assets/images/Run2.png");
    iLoadImage(&runFrames[3], "assets/images/Run3.png");
    iLoadImage(&runFrames[4], "assets/images/Run4.png");
    iLoadImage(&runFrames[5], "assets/images/Run5.png");
    iLoadImage(&runFrames[6], "assets/images/Run6.png");
    iLoadImage(&runFrames[7], "assets/images/Run7.png");
    iLoadImage(&runFrames[8], "assets/images/Run8.png");
    iLoadImage(&runFrames[9], "assets/images/Run9.png");
    //Resize Run Image
    double runRatio;
    for(int i=0; i<10; i++){
        runRatio = (double)runFrames[i].width / runFrames[i].height;
        iResizeImage(&runFrames[i], 100, 100.0 / runRatio);
    }
    // Load jumping frames
    iLoadImage(&jumpFrames[0], "assets/images/Jump0.png");
    iLoadImage(&jumpFrames[1], "assets/images/Jump1.png");
    iLoadImage(&jumpFrames[2], "assets/images/Jump2.png");
    iLoadImage(&jumpFrames[3], "assets/images/Jump3.png");
    iLoadImage(&jumpFrames[4], "assets/images/Jump4.png");
    iLoadImage(&jumpFrames[5], "assets/images/Jump5.png");
    iLoadImage(&jumpFrames[6], "assets/images/Jump6.png");
    iLoadImage(&jumpFrames[7], "assets/images/Jump7.png");
    iLoadImage(&jumpFrames[8], "assets/images/Jump8.png");
    iLoadImage(&jumpFrames[9], "assets/images/Jump9.png");
    //Resize Jump Images
    double jumpRatio;
    for(int i=0; i<10; i++){
        jumpRatio = (double)jumpFrames[i].width / jumpFrames[i].height;
        iResizeImage(&jumpFrames[i], 100, 100.0 / jumpRatio);
    }
    // Load sliding frames
    iLoadImage(&slideFrames[0], "assets/images/Slide0.png");
    iLoadImage(&slideFrames[1], "assets/images/Slide1.png");
    iLoadImage(&slideFrames[2], "assets/images/Slide2.png");
    iLoadImage(&slideFrames[3], "assets/images/Slide3.png");
    iLoadImage(&slideFrames[4], "assets/images/Slide4.png");
    iLoadImage(&slideFrames[5], "assets/images/Slide5.png");
    iLoadImage(&slideFrames[6], "assets/images/Slide6.png");
    iLoadImage(&slideFrames[7], "assets/images/Slide7.png");
    iLoadImage(&slideFrames[8], "assets/images/Slide8.png");
    iLoadImage(&slideFrames[9], "assets/images/Slide9.png");
    //Resize Sliding Images
    double slideRatio;
    for(int i=0; i<10; i++){
        slideRatio = (double)slideFrames[i].width / slideFrames[i].height;
        iResizeImage(&slideFrames[i], 100, 100.0 / slideRatio);
    }
    // Load death frames
    iLoadImage(&deathFrames[0], "assets/images/Dead0.png");
    iLoadImage(&deathFrames[1], "assets/images/Dead1.png");
    iLoadImage(&deathFrames[2], "assets/images/Dead2.png");
    iLoadImage(&deathFrames[3], "assets/images/Dead3.png");
    iLoadImage(&deathFrames[4], "assets/images/Dead4.png");
    iLoadImage(&deathFrames[5], "assets/images/Dead5.png");
    iLoadImage(&deathFrames[6], "assets/images/Dead6.png");
    iLoadImage(&deathFrames[7], "assets/images/Dead7.png");
    iLoadImage(&deathFrames[8], "assets/images/Dead8.png");
    iLoadImage(&deathFrames[9], "assets/images/Dead9.png");
    //Resize Dead Images
    double deadRatio;
    for(int i=0; i<10; i++){
        deadRatio = (double)deathFrames[i].width / deathFrames[i].height;
        iResizeImage(&deathFrames[i], 100, 100.0 / deadRatio);
    }
    //Load and resize obstacle image
    iLoadImage(&obstacleImg, "assets/images/obs1.png");
    double obsRatio = (double)obstacleImg.width / obstacleImg.height;
	iResizeImage(&obstacleImg, 48, 48.0 / obsRatio);

	iLoadImage(&bg, "assets/images/bg1.jpg");
	iResizeImage(&bg, 1200, 800);
}

int main()
{
    loadResources();
    moveTimer = iSetTimer(30, moveObstacle);
    jumpTimer = iSetTimer(30, jumpUpdate);
    frameTimer = iSetTimer(80, updateFrame);
    srand(time(0));
    iWindowedMode(1200, 800, "Road Runner");
    iStartMainLoop();
    return 0;
}
