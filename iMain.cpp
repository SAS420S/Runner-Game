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
Image box0, box1, BigBox, MiniBox;

//Game variables
int gameState = 0;

int playerX = 150, playerY = 150;
int bgX1 = 0, bgX2 = 1200;

int jumpY = 0;
int isJumping = 0;
int jumpUp = 1;

int isSliding = 0, slideCount = 0;
int playerFrame = 0;

int obsX = 1200, obsY = 150, obsW = 40, obsH = 40;
int obsType = 1;
int Speed;

int gameOver = 0, music = 1, pause = 0;
int score = 0, high[4];
int isHigh;
int TimerIndex[4];
char str[40];

void scoreUpdate(){
    if(gameState == 1 && gameOver == 0){
        score++;
    }
}

int updateHigh()
{
    int temp;
    high[3] = score;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3 - i; j++){
            if(high[j] < high[j + 1]){
                temp = high[j];
                high[j] = high[j+1];
                high[j + 1] = temp;
            }
        }
    }
    FILE *fp = fopen("high.txt", "w");
    fprintf(fp, "%d %d %d", high[0], high[1], high[2]);
    fclose(fp);
    if(score == high[0]){
        return 1;
    }
    else if(score == high[1]){
        return 2;
    }
    else if(score == high[2]){
        return 3;
    }
    else{
        return 0;
    }
}

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
    score = 0;
}

void createNewObstacle(){
    obsX = (rand()%500) + 1200;
    if(rand() % 2 == 0){
        obsType = 1;
    }
    else{
        obsType = 2;
    }
    if(obsType == 2){
        obsH = 20;
        obsY = 250;
    }
    else{
        obsH = 40;
        obsY = 150;
    }
}

//Move Obstacle, Background + Collision Chake
void moveObsbg()
{
    if(gameState != 1){
        return;
    }
    if(gameOver){
        return;
    }
    Speed = 10 + score / 50;
    if(Speed >= 20){
        Speed = 20;
    }
    obsX -= Speed;
    bgX1 -= Speed;
    bgX2 -= Speed;
    if(obsX + obsW < 0) {
        createNewObstacle();
    }
    int px = playerX;
    int py = playerY + jumpY;
    int ph = isSliding ? 20 : 210;
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
        else{
            gameOver = 1;
            playerFrame = 0;
            isHigh = updateHigh();
            if(music){
                iPlaySound("assets/sounds/gameover.wav", 0, 80);
            }
        }
    }
}

void jumpSlideUpdate(){
    if(gameState != 1) return;
    if(isSliding){
        slideCount++;
        if(slideCount >= 23){
            isSliding = 0;
            slideCount = 0;
        }
    }
    if(isJumping){
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
}

void iDraw(){
    iClear();
    // MENU
    if(gameState == 0) {
        iShowLoadedImage(0, 0, &mbg);
        iShowLoadedImage(25, 600, &info);
        if(music){
            iShowLoadedImage(25, 650, &muson);
        }
        else{
            iShowLoadedImage(25, 650, &musoff);
        }
        iSetColor(0, 0, 0);
        iShowText(325, 570, "ROAD RUNNER", "assets/fonts/Sixtyfour-Regular-VariableFont_BLED,SCAN.ttf", 45);
        iShowLoadedImage(450, 150, &box1);
        iShowText(530, 173, "QUIT", "assets/fonts/Antonio-Bold.ttf", 30);
        iShowLoadedImage(450, 250, &box1);
        iShowText(480, 275, "INSTRUCTIONS", "assets/fonts/Antonio-Bold.ttf", 28);
        iShowLoadedImage(450, 350, &box1);
        iShowText(490, 375, "HIGH SCORE", "assets/fonts/Antonio-Bold.ttf", 28);
        iShowLoadedImage(450, 450, &box1);
        iShowText(520, 473, "START", "assets/fonts/Antonio-Bold.ttf", 30);
        return;
    }
    // GAME OVER
    if(gameOver){
        iShowLoadedImage(bgX1, 0, &bg);
        iShowLoadedImage(bgX2, 0, &bg);
        iShowLoadedImage(playerX, playerY, &deathFrames[playerFrame]);
        iShowLoadedImage(300, 100, &BigBox);
        iSetColor(0, 0, 0);
        iShowText(440, 525, "GAME OVER", "assets/fonts/Sixtyfour-Regular-VariableFont_BLED,SCAN.ttf", 45);
        iShowLoadedImage(385, 210, &box1);
        iShowLoadedImage(405, 227, &restart);
        iShowText(455, 235, "RESTART", "assets/fonts/Antonio-Bold.ttf", 30);
        iShowLoadedImage(760, 210, &box1);
        iShowLoadedImage(783, 227, &menu);
        iShowText(830, 235, "MENU", "assets/fonts/Antonio-Bold.ttf", 30);
        sprintf(str, " SCORE: %d", score);
        iSetColor(255, 255, 255);
        iShowText(570, 420, str, "assets/fonts/Antonio-Bold.ttf", 40);
        if(isHigh == 1){
            iShowText(470, 360, "Incredible\! You\'ve take the top high score\!", "assets/fonts/Antonio-Bold.ttf", 25);
        }
        else if(isHigh == 2){
            iShowText(470, 360, "Great\! You reached the 2nd highest score\!", "assets/fonts/Antonio-Bold.ttf", 25);
        }
        else if(isHigh == 3){
            iShowText(470, 360, "Nice\! You made it to top 3 high scores\!", "assets/fonts/Antonio-Bold.ttf", 25);
        }
        return;
    }
    if(gameState == 2){
        iShowLoadedImage(0, 0, &mbg);
        iShowLoadedImage(230, 120, &BigBox);
        iSetColor(0, 0, 0);
        iShowText(400, 550, "HIGH SCORE", "assets/fonts/Sixtyfour-Regular-VariableFont_BLED,SCAN.ttf", 45);
        iShowLoadedImage(400, 425, &MiniBox);
        iShowText(426, 450, "1", "assets/fonts/Antonio-Bold.ttf", 20);
        iShowLoadedImage(400, 350, &MiniBox);
        iShowText(426, 375, "2", "assets/fonts/Antonio-Bold.ttf", 20);
        iShowLoadedImage(400, 270, &MiniBox);
        iShowText(426, 295, "3", "assets/fonts/Antonio-Bold.ttf", 20);
        iSetColor(255, 255, 255);
        sprintf(str, "%d", high[0]);
        iShowText(500, 445, str, "assets/fonts/Antonio-Bold.ttf", 30);
        sprintf(str, "%d", high[1]);
        iShowText(500, 370, str, "assets/fonts/Antonio-Bold.ttf", 30);
        sprintf(str, "%d", high[2]);
        iShowText(500, 290, str, "assets/fonts/Antonio-Bold.ttf", 30);

        iShowLoadedImage(470, 70, &box1);
        iSetColor(0, 0, 0);
        iShowText(540, 92, "BACK", "assets/fonts/Antonio-Bold.ttf", 30);
        return;
    }
    if(gameState == 3){
        iShowLoadedImage(0, 0, &mbg);
        iShowLoadedImage(230, 120, &BigBox);
        iSetColor(0, 0, 0);
        iShowText(340, 560, "INSTRUCTIONS", "assets/fonts/Sixtyfour-Regular-VariableFont_BLED,SCAN.ttf", 45);
        iShowLoadedImage(470, 70, &box1);
        iShowText(540, 92, "BACK", "assets/fonts/Antonio-Bold.ttf", 30);

        iSetColor(255, 255, 255);
        iShowText(350, 450, "Press \"UP\" Button for Jump", "assets/fonts/Antonio-Bold.ttf", 25);
        iShowText(350, 400, "Press \"DOWN\" Button for Slide", "assets/fonts/Antonio-Bold.ttf", 25);
        iShowText(350, 350, "Press \"SPACE\" Button for Pause the Game", "assets/fonts/Antonio-Bold.ttf", 25);
        iShowText(350, 300, "Press \"SPACE\" Button for Resume the Game", "assets/fonts/Antonio-Bold.ttf", 25);
        return;
    }
    if(gameState == 4){
        iShowLoadedImage(0, 0, &mbg);
        iSetColor(0, 0, 0);
        iShowText(325, 630, "ROAD RUNNER", "assets/fonts/Sixtyfour-Regular-VariableFont_BLED,SCAN.ttf", 45);
        iShowLoadedImage(470, 70, &box1);
        iShowText(540, 92, "BACK", "assets/fonts/Antonio-Bold.ttf", 30);
        char about1[] = "This is a simple 2D endless runner game developed using C and iGraphics for PROG 102 project. The player must avoid obstacles";
        char about2[] = "by jumping or sliding. The game ends when the player hits an obstacle. The goal is to survive as long as possible and achieve";
        iText(60, 590, about1, GLUT_BITMAP_HELVETICA_18);
        iText(60, 560, about2, GLUT_BITMAP_HELVETICA_18);
        iText(60, 530, "a high score.", GLUT_BITMAP_HELVETICA_18);
        iText(810, 410, "Special thanks to: ", GLUT_BITMAP_HELVETICA_18);
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
        iShowLoadedImage(playerX, playerY, &runFrames[playerFrame]);
    }
    sprintf(str, " Score: %d", score);
    iText(1050, 650, str, GLUT_BITMAP_HELVETICA_18);
    switch(obsType)
    {
        case 2: iShowLoadedImage(obsX, obsY, &obstacleImg2); break;
        default: iShowLoadedImage(obsX, obsY, &obstacleImg);
    }
}

void iKeyPress(unsigned char key){
    if(gameState == 1 && key == ' '){
        if(!pause){
            pause = 1;
            for(int i = 0; i < 4; i++){
                iPauseTimer(TimerIndex[i]);
            }
        }
        else{
            pause = 0;
            for(int i = 0; i < 4; i++){
                iResumeTimer(TimerIndex[i]);
            }
        }
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
        if(gameState == 0){
            if(mx >= 455 && mx <= 645 && my >= 456 && my <= 510){
                if(music){
                    iPlaySound("assets/sounds/click.wav", 0, 80);
                }
                gameState = 1;
            }
            else if(mx >= 455 && mx <= 645 && my >= 356 && my <= 410){
                if(music){
                    iPlaySound("assets/sounds/click.wav", 0, 80);
                }
                gameState = 2;
            }
            else if(mx >= 455 && mx <= 645 && my >= 256 && my <= 310){
                if(music){
                    iPlaySound("assets/sounds/click.wav", 0, 80);
                }
                gameState = 3;
            }
            else if(mx >= 25 && mx <= 68 && my >= 655 && my <= 692){
                if(music){
                    iPlaySound("assets/sounds/click.wav", 0, 80);
                    music = 0;
                }
                else{
                    music = 1;
                }
            }
            else if(mx >= 25 && mx <= 68 && my >= 604 && my <= 644){
                if(music){
                    iPlaySound("assets/sounds/click.wav", 0, 80);
                }
                gameState = 4;
            }
            else if(mx >= 455 && mx <= 645 && my >= 156 && my <= 210){
                if(music){
                    iPlaySound("assets/sounds/click.wav", 0, 80);
                }
                iExitMainLoop();
            }
        }
        else if(gameOver){
            if(mx >= 390 && mx <= 580 && my >= 215 && my <= 270){
                if(music){
                    iPlaySound("assets/sounds/click.wav", 0, 80);
                }
                resetGame();
            }
            else if(mx >= 765 && mx <= 955 && my >= 215 && my <= 270){
                if(music){
                    iPlaySound("assets/sounds/click.wav", 0, 80);
                }
                gameState = 0;
                resetGame();
            }
        }
        if(gameState == 2 || gameState == 3 || gameState == 4){
            if(mx >= 475 && mx <= 665 && my >= 75 && my <= 130){
                if(music){
                    iPlaySound("assets/sounds/click.wav", 0, 80);
                }
                gameState = 0;
            }
        }
    }
}

// Load and Resize Game Images
void loadImages()
{
    float Ratio;
    char file[50];
    for(int i=0; i<10; i++){
        sprintf(file, "assets/images/Run%d.png", i);
        iLoadImage(&runFrames[i], file);
        Ratio = (float)runFrames[i].width / runFrames[i].height;
        iResizeImage(&runFrames[i], 100, 100.0 / Ratio);
    }
    for(int i=0; i<10; i++){
        sprintf(file, "assets/images/Jump%d.png", i);
        iLoadImage(&jumpFrames[i], file);
        Ratio = (float)jumpFrames[i].width / jumpFrames[i].height;
        iResizeImage(&jumpFrames[i], 100, 100.0 / Ratio);
    }
    for(int i=0; i<10; i++){
        sprintf(file, "assets/images/Slide%d.png", i);
        iLoadImage(&slideFrames[i], file);
        Ratio = (float)slideFrames[i].width / slideFrames[i].height;
        iResizeImage(&slideFrames[i], 100, 100.0 / Ratio);
    }
    for(int i=0; i<10; i++){
        sprintf(file, "assets/images/Dead%d.png", i);
        iLoadImage(&deathFrames[i], file);
        Ratio = (float)deathFrames[i].width / deathFrames[i].height;
        iResizeImage(&deathFrames[i], 120, 120.0 / Ratio);
    }
    iLoadImage(&obstacleImg, "assets/images/obs1.png");
    Ratio = (float)obstacleImg.width / obstacleImg.height;
	iResizeImage(&obstacleImg, 48, 48.0 / Ratio);
    iLoadImage(&obstacleImg2, "assets/images/obs2.png");
    Ratio = (float)obstacleImg2.width / obstacleImg2.height;
	iResizeImage(&obstacleImg2, 150, 150.0 / Ratio);

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
	iResizeImage(&menu, 42, 42);
    iLoadImage(&restart, "assets/images/restart.png");
	iResizeImage(&restart, 42, 42);

    iLoadImage(&box0, "assets/images/sbox.png");
    Ratio = (float)box0.width / box0.height;
	iResizeImage(&box0, 150, 150.0 / Ratio);
    iLoadImage(&box1, "assets/images/sbox2.png");
    Ratio = (float)box1.width / box1.height;
	iResizeImage(&box1, 200, 200.0 / Ratio);
    iLoadImage(&BigBox, "assets/images/bigbox.png");
    Ratio = (float)BigBox.width / BigBox.height;
	iResizeImage(&BigBox, 750, 750.0 / Ratio);
	iLoadImage(&MiniBox, "assets/images/minibox.png");
	iResizeImage(&MiniBox, 60, 60);
}

int main()
{

    FILE *fptr = fopen("high.txt", "r");
    fscanf(fptr, "%d %d %d", &high[0], &high[1], &high[2]);
    fclose(fptr);
    loadImages();
    TimerIndex[0] = iSetTimer(30, moveObsbg);
    TimerIndex[1] = iSetTimer(30, jumpSlideUpdate);
    TimerIndex[2] = iSetTimer(80, updateFrame);
    TimerIndex[3] = iSetTimer(500, scoreUpdate);
    srand(time(0));
    iWindowedMode(1200, 800, "Road Runner");
    iStartMainLoop();
    return 0;
}
    
