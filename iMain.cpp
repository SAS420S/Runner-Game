#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "iGraphics.h"
#include "iFont.h"
#define SDL_MAIN_HANDLED
#include "iSound.h"

//Image variables
Image runFrames[10];
Image jumpFrames[10];
Image slideFrames[10];
Image deathFrames[10];
Image obstacleImg, obstacleImg2, bg, mbg;
Image info, muson, musoff, menu, restart;
Image box0, box1, BigBox;

// GAME VARIABLES
int gameState = 0;

int playerX = 150, playerY = 150;
int bgX1 = 0, bgX2 = 1200;

int jumpY = 0;
int isJumping = 0;
int jumpUp = 1;

int isSliding = 0;
int playerFrame = 0;

int obsX = 1200, obsY = 150, obsW = 40, obsH = 40;

int obsType = 1;

int gameOver = 0, music = 1, pause = 0;
int score = 0;

void updateFrame()
{
    if(gameState != 1) return;
    if(gameOver) {
        // Death animation runs slower
        if(playerFrame < 9) playerFrame++;
        return;
    }
    playerFrame++;
    if(playerFrame >= 10) playerFrame = 0;
}

void resetGame()
{
    gameOver = 0;
    obsX = 1200;
    jumpY = 0;
    isJumping = 0;
    jumpUp = 1;
    isSliding = 0;
    playerFrame = 0;
}

void createNewObstacle() {
    obsX = (rand()%500) + 1200;
    if(rand() % 2 == 0) obsType = 1;
    else obsType = 2;
    if(obsType == 2) {
        obsH = 20;
        obsY = 200;
    }
    else {
        obsH = 40;
        obsY = 150;
    }
}

// MOVE OBSTACLE + COLLISION CHECK
void moveObsbg() {
    if(gameState != 1) return;
    if(gameOver) return;
    obsX -= 10;
    bgX1 -= 10;
    bgX2 -= 10;
    if(obsX + obsW < 0) {
        createNewObstacle();
    }
    int px = playerX;
    int py = playerY + jumpY;
    int ph = isSliding ? 20 : 40;
    int ox = obsX;
    int oy = obsY;
    if(bgX1 + 1200 <= 0){
        bgX1 = bgX2 + 1200;
    }
    if(bgX2 + 1200 <= 0){
        bgX2 = bgX1 + 1200;
    }
    if(px < ox + obsW && px + 40 > ox && py < oy + obsH && py + ph > oy) {
        if(obsType == 2 && isSliding) {
            // SAFE
        }
        else {
            gameOver = 1;
            playerFrame = 0;
        }
    }
}

void jumpUpdate() {
    if(gameState != 1) return;
    if(!isJumping) return;
    if(isSliding && obsX + obsW < playerX){
        isSliding = 0;
        return;
    }
    if(jumpUp){
        jumpY += 10;
        if(jumpY >= 120) jumpUp = 0;
    }
    else{
        jumpY -= 10;
        if(jumpY <= 0){
            jumpY = 0;
            isJumping = 0;
            jumpUp = 1;
        }
    }
}

void scoreUpdate(){
    if(gameState == 1 && gameOver == 0){
        score++;
    }
}

void iDraw(){
    iClear();
    // MENU
    if(gameState == 0) {
        iShowLoadedImage(0, 0, &mbg);
        iShowLoadedImage(25, 600, &info);
        iShowLoadedImage(25, 650, &muson);
        iShowLoadedImage(550, 150, &box1);
        iShowLoadedImage(550, 300, &box1);
        iShowLoadedImage(550, 450, &box1);
        iShowLoadedImage(550, 550, &box1);
        iText(300, 400, "ROAD RUNNER", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(260, 340, "Press SPACE to Start", GLUT_BITMAP_HELVETICA_18);
        return;
    }
    // GAME OVER
    if(gameOver){
        iShowLoadedImage(bgX1, 0, &bg);
        iShowLoadedImage(bgX2, 0, &bg);
        iShowLoadedImage(playerX, playerY, &deathFrames[playerFrame]);
        iText(350, 300, "GAME OVER", GLUT_BITMAP_HELVETICA_18);
        iText(300, 260, "Press R to Restart", GLUT_BITMAP_HELVETICA_18);
        return;
    }
    iShowLoadedImage(bgX1, 0, &bg);
    iShowLoadedImage(bgX2, 0, &bg);
    if(isJumping){
        iShowLoadedImage(playerX, playerY + jumpY, &jumpFrames[playerFrame]);
    }
    else if(isSliding){
        iShowLoadedImage(playerX, playerY, &slideFrames[playerFrame]);
    }
    else{
        iPlaySound("assets/sounds/running.wav", 0, 80);
        iShowLoadedImage(playerX, playerY, &runFrames[playerFrame]);
    }
    char str[40];
    sprintf(str, " Score: %d", score);
    iText(100, 600, str, GLUT_BITMAP_HELVETICA_18);
    switch(obsType)
    {
        case 2: iShowLoadedImage(obsX, obsY, &obstacleImg2); break;
        default: iShowLoadedImage(obsX, obsY, &obstacleImg);
    }
}

void iKeyPress(unsigned char key){
    if(gameState == 0){
        if(key == ' ') gameState = 1;
        return;
    }
    if(gameState == 1 && key == ' '){
        if(!pause){
            pause = 1;
            iPauseTimer(0);
        }
        else{
            pause = 0;
            iResumeTimer(0);
        }
    }
    if((key == 'r' || key == 'R') && gameOver){
        resetGame();
    }
}


void iSpecialKeyPress(unsigned char key)
{
    if(gameState == 1){
        switch(key){
            case GLUT_KEY_UP:
                if(!isJumping && !isSliding && !gameOver){
                    isJumping = 1;
                    if(music){
                        iPlaySound("assets/sounds/jumping.wav", 0, 80);
                    }
                    jumpUp = 1;
                    playerFrame = 0;
                }
            case GLUT_KEY_DOWN:
                if(!isJumping && !gameOver){
                    isSliding = 1;
                    if(music){
                        iPlaySound("assets/sounds/slide.wav", 0, 80);
                    }
                    playerFrame = 0;
                }
            default: break;
        }
    }
}

void iMouseClick(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(mx >= 300 && mx <= 450 && my >= 250 && my <= 300)
        {
            printf("Start Button Clicked\n");
        }
    }
}

// Load and Resize Game Images
void loadResources()
{
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
    double runRatio;
    for(int i=0; i<10; i++){
        runRatio = (double)runFrames[i].width / runFrames[i].height;
        iResizeImage(&runFrames[i], 100, 100.0 / runRatio);
    }
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
    double jumpRatio;
    for(int i=0; i<10; i++){
        jumpRatio = (double)jumpFrames[i].width / jumpFrames[i].height;
        iResizeImage(&jumpFrames[i], 100, 100.0 / jumpRatio);
    }
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
    double slideRatio;
    for(int i=0; i<10; i++){
        slideRatio = (double)slideFrames[i].width / slideFrames[i].height;
        iResizeImage(&slideFrames[i], 100, 100.0 / slideRatio);
    }
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
    double deadRatio;
    for(int i=0; i<10; i++){
        deadRatio = (double)deathFrames[i].width / deathFrames[i].height;
        iResizeImage(&deathFrames[i], 100, 100.0 / deadRatio);
    }
    iLoadImage(&obstacleImg, "assets/images/obs1.png");
    double obsRatio = (double)obstacleImg.width / obstacleImg.height;
	iResizeImage(&obstacleImg, 48, 48.0 / obsRatio);
    iLoadImage(&obstacleImg2, "assets/images/obs2.png");
    obsRatio = (double)obstacleImg2.width / obstacleImg2.height;
	iResizeImage(&obstacleImg2, 48, 48.0 / obsRatio);

	iLoadImage(&bg, "assets/images/bg.jpg");
	iResizeImage(&bg, 1200, 800);
	iLoadImage(&mbg, "assets/images/mbg.jpg");
	iResizeImage(&mbg, 1200, 800);

	iLoadImage(&info, "assets/images/info.png");
	iResizeImage(&info, 48, 48);
	iLoadImage(&muson, "assets/images/muson.png");
	iResizeImage(&muson, 48, 48);
	iLoadImage(&musoff, "assets/images/musoff.png");
	iResizeImage(&musoff, 48, 48);
    iLoadImage(&menu, "assets/images/menu.png");
	iResizeImage(&menu, 48, 48);
    iLoadImage(&restart, "assets/images/restart.png");
	iResizeImage(&restart, 48, 48);
    float Ratio;
    iLoadImage(&box0, "assets/images/sbox.png");
    Ratio = (float)box0.width / box0.height;
	iResizeImage(&box0, 150, 150.0 / Ratio);
    iLoadImage(&box1, "assets/images/sbox2.png");
    Ratio = (float)box1.width / box1.height;
	iResizeImage(&box1, 100, 100.0 / Ratio);
    iLoadImage(&BigBox, "assets/images/bigbox.png");
    Ratio = (float)BigBox.width / BigBox.height;
	iResizeImage(&BigBox, 150, 150.0 / Ratio);
}

int main()
{
    loadResources();
    moveTimer = iSetTimer(30, moveObsbg);
    jumpTimer = iSetTimer(30, jumpUpdate);
    frameTimer = iSetTimer(80, updateFrame);
    iSetTimer(500, scoreUpdate);
    srand(time(0));
    iWindowedMode(1200, 800, "Road Runner");
    iStartMainLoop();
    return 0;
}
    
