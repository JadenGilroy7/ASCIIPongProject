/*
* ASCII Pong
*    Inspired by "Code-It-Yourself! Tetris - Programming from Scratch (Quick and Simple C++)": https://youtu.be/8OK8_tHeCIA
*/

#include <Windows.h>
#include <chrono>
#include <thread>
#include "ASCIIPongProject.h"

const int SCREEN_WIDTH = 120;    // Console Screen Size X (columns)
const int SCREEN_HEIGHT = 30;    // Console Screen Size Y (rows)

//Since we only use one character to display the score of each player, the winning score should be smaller than 10
const unsigned int WINNING_SCORE = 3;
const unsigned int SCORE_X_POSITION = 15;
const unsigned int SCORE_Y_POSITION = 2;
unsigned int player1Score = 0;
unsigned int player2Score = 0;

//WINNER_X_POSITION is measured from the left border for player 1 and from the right border for player 2
const unsigned int WINNER_X_POSITION = SCREEN_WIDTH / 4;
const unsigned int WINNER_Y_POSITION = SCREEN_HEIGHT / 2;
const unsigned int FINAL_GAMEOVER_PAUSE = 5000; //milliseconds

const unsigned int BALL_SPEED = 5;

const unsigned int PADDLE_DISTANCE_FROM_SIDE_BORDERS = 2;
const unsigned int PADDLE_HEIGHT = 8;

//Paddle one XY position
unsigned int paddle1XPos = PADDLE_DISTANCE_FROM_SIDE_BORDERS;
unsigned int paddle1YPos = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;

//Paddle two XY position
unsigned int paddle2XPos = (SCREEN_WIDTH - 1) - PADDLE_DISTANCE_FROM_SIDE_BORDERS;
unsigned int paddle2YPos = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;

int main()
{
    /*
    * Initialize data structures.
    */

    //Ball XY position
    unsigned int ballXPos = SCREEN_WIDTH / 2;
    unsigned int ballYPos = SCREEN_HEIGHT / 2;

    //Ball XY direction
    int ballXSpeed = 1; //go right
    int ballYSpeed = 1; //go down

    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    wchar_t* screen = new wchar_t[SCREEN_WIDTH * SCREEN_HEIGHT];

    // Main Loop
    bool bGameOver = false;
    while (!bGameOver)
    {
        // Draw a blank field of size nScreenWidth * nScreenHeight
        drawBlankField(screen);

        //Draw left, center and right vertical borders
        drawVerticalBars(screen);

        //Draw horizontal lines at the top and bottom of the screen
        drawHorizontalBorders(screen);

        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(BALL_SPEED)); // Small Step = 1 Game Tick

        // Read user input (pressing W/S or O/K keys)
        wchar_t controls[] = { L'W', L'S', L'O', L'K' };
        bool bKey[sizeof(controls) / sizeof(wchar_t)];
        for (int k = 0; k < sizeof(bKey) / sizeof(bool); k++)
            bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)(controls[k]))) != 0;

        // Handle paddle one movement
        handlePaddleOneMovement(screen, bKey);

        // Draw paddle one
        drawPaddleOne(screen);

        // Handle paddle two movement
        handlePaddleTwoMovement(screen, bKey);

        // Draw paddle two
        drawPaddleTwo(screen);

        // Reposition Ball vertically
        repositionBallVertically(ballYPos, ballYSpeed);

        // Reposition Ball horizontally
        repositionBallHorizontally(ballYPos, ballXPos, ballXSpeed);

        // Draw Player 1 and Player 2 Scores
        printPlayerScores(screen);

        //Draw ball
        screen[ballXPos + ballYPos * SCREEN_WIDTH] = L'O';

        checkWinningConditions(screen, bGameOver);

        // Display Frame
        WriteConsoleOutputCharacter(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0,0 }, &dwBytesWritten);
    }

    // Exiting
    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(FINAL_GAMEOVER_PAUSE));
    CloseHandle(hConsole);
    return 0;
}

void drawBlankField(wchar_t* screen)
{
    for (size_t index = 0; index < SCREEN_HEIGHT * SCREEN_WIDTH; index++)
    {
        screen[index] = L' ';
    }
}

void drawVerticalBars(wchar_t* screen)
{
    for (size_t index = 0; index < SCREEN_HEIGHT * SCREEN_WIDTH; index += SCREEN_WIDTH)
    {
        screen[index] = L'|';
    }

    for (size_t index = SCREEN_WIDTH / 2; index < SCREEN_HEIGHT * SCREEN_WIDTH; index += SCREEN_WIDTH)
    {
        screen[index] = L'.';
    }

    for (size_t index = SCREEN_WIDTH - 1; index < SCREEN_HEIGHT * SCREEN_WIDTH; index += SCREEN_WIDTH)
    {
        screen[index] = L'|';
    }
}

void drawHorizontalBorders(wchar_t* screen)
{
}
void handlePaddleOneMovement(wchar_t* screen, const bool bKey[4])
{
}

void handlePaddleTwoMovement(wchar_t* screen, const bool bKey[4])
{
}

void drawPaddleOne(wchar_t* screen)
{
    for (size_t index = 0; index < PADDLE_HEIGHT; index++)
    {
        screen[(paddle1YPos + index) * SCREEN_WIDTH + paddle1XPos] = L'>';
    }
}

void drawPaddleTwo(wchar_t* screen)
{
}

void repositionBallVertically(unsigned int& ballYPos, int& ballYSpeed)
{
    if (ballYSpeed > 0) //ball going down
    {
        if (ballYPos >= (SCREEN_HEIGHT - 1) - 1)
        {
            ballYSpeed *= -1;
        }
        ballYPos += ballYSpeed;
    }
    else if (ballYSpeed < 0) //ball going up
    {
        if (ballYPos <= 1)
        {
            ballYSpeed *= -1;
        }
        ballYPos += ballYSpeed;
    }
}

void repositionBallHorizontally(const unsigned int ballYPos, unsigned int& ballXPos, int& ballXSpeed)
{
}

void printPlayerScores(wchar_t* screen)
{
}

void checkWinningConditions(wchar_t* screen, bool& bGameOver)
{
}

void printPlayerOneWon(wchar_t* screen, const wchar_t* text, const unsigned int textLength)
{
}

void printPlayerTwoWon(wchar_t* screen, const wchar_t* text, const unsigned int textLength)
{
}