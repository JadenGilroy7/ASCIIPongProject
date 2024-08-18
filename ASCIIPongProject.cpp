/*
* ASCII Pong
*    Inspired by "Code-It-Yourself! Tetris - Programming from Scratch (Quick and Simple C++)": https://youtu.be/8OK8_tHeCIA
*/

#include <Windows.h>
#include <chrono>
#include <thread>
#include "ASCIIPongProject.h"
#include <conio.h>

const int SCREEN_WIDTH = 120;    // Console Screen Size X (columns)
const int SCREEN_HEIGHT = 30;    // Console Screen Size Y (rows)

//Since we only use one character to display the score of each player, the winning score should be smaller than 10
const unsigned int WINNING_SCORE = 3;
const unsigned int SCORE_X_POSITION = 15;
const unsigned int SCORE_Y_POSITION = 2;
unsigned int player1Score = 0;
unsigned int player2Score = 0;

//WINNER_X_POSITION is measured from the left border for player 1 and from the right border for player 2
const unsigned int WINNER_X_POSITION = SCREEN_WIDTH / 2;
const unsigned int WINNER_Y_POSITION = SCREEN_HEIGHT / 2;
const unsigned int FINAL_GAMEOVER_PAUSE = 5000; //milliseconds

const unsigned int BALL_SPEED = 15; // Delay between movement ticks

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

    bool gamePlaying = true;

    while (gamePlaying) // Game restart loop
    {
        bool bGameOver = false;
        resetGame(ballXPos, ballYPos, ballXSpeed, ballYSpeed);

        // Main game loop
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
            repositionBallHorizontally(ballYPos, ballXPos, ballXSpeed, ballYSpeed);

            // Draw Player 1 and Player 2 Scores
            printPlayerScores(screen);

            //Draw ball
            screen[ballXPos + ballYPos * SCREEN_WIDTH] = L'O';

            checkWinningConditions(screen, bGameOver);

            // Display Frame
            WriteConsoleOutputCharacter(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0,0 }, &dwBytesWritten);
        }

        // Game over screen
        drawBlankField(screen);
        if (player1Score >= WINNING_SCORE)
        {
            printPlayerWon(screen, L"Player 1 Wins!", 14);
        }
        else if (player2Score >= WINNING_SCORE)
        {
            printPlayerWon(screen, L"Player 2 Wins!", 14);
        }

        // Display "Press any key to restart" message
        const wchar_t* restartMsg = L"Press any key to restart";
        unsigned int msgLength = 24;
        unsigned int msgXPosition = (SCREEN_WIDTH - msgLength) / 2;
        unsigned int msgYPosition = SCREEN_HEIGHT / 2 + 2;
        for (unsigned int i = 0; i < msgLength; i++)
        {
            screen[msgXPosition + i + msgYPosition * SCREEN_WIDTH] = restartMsg[i];
        }

        // Display final frame
        WriteConsoleOutputCharacter(hConsole, screen, SCREEN_WIDTH * SCREEN_HEIGHT, { 0,0 }, &dwBytesWritten);

        // Wait for any key press
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
        _getch();
    }

    // Cleanup
    CloseHandle(hConsole);
    delete[] screen;
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
    // Draw top border
    for (int i = 0; i < SCREEN_WIDTH; i++)
    {
        screen[i] = L'-';
    }

    //Draw bottom borde
    for (int i = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH; i < SCREEN_HEIGHT * SCREEN_WIDTH; i++)
    {
        screen[i] = L'-';
    }

}
void handlePaddleOneMovement(wchar_t* screen, const bool bKey[4])
{
    // Move up
    if (bKey[0] && paddle1YPos > 1)
    {
        paddle1YPos--;
    }

    // Move down
    if (bKey[1] && paddle1YPos < SCREEN_HEIGHT - PADDLE_HEIGHT - 1)
    {
        paddle1YPos++;
    }
}

void handlePaddleTwoMovement(wchar_t* screen, const bool bKey[4])
{
    // Move up
    if (bKey[2] && paddle2YPos > 1)
    {
        paddle2YPos--;
    }

    // Move down
    if (bKey[3] && paddle2YPos < SCREEN_HEIGHT - PADDLE_HEIGHT - 1)
    {
        paddle2YPos++;
    }
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
    for (size_t index = 0; index < PADDLE_HEIGHT; index++)
    {
        screen[(paddle2YPos + index) * SCREEN_WIDTH + paddle2XPos] = L'<';
    }
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

void repositionBallHorizontally(const unsigned int ballYPos, unsigned int& ballXPos, int& ballXSpeed, int& ballYSpeed)
{
    if (ballXSpeed > 0) // ball going right
    {
        if (ballXPos >= paddle2XPos - 1)
        {
            if (ballYPos >= paddle2YPos && ballYPos < paddle2YPos + PADDLE_HEIGHT)
            {
                ballXSpeed *= -1;

                // Adjust vertical speed based on where the ball hits the paddle
                int hitPosition = ballYPos - paddle2YPos;
                if (hitPosition < PADDLE_HEIGHT / 3)
                    ballYSpeed = -1;
                else if (hitPosition < 2 * PADDLE_HEIGHT / 3)
                    ballYSpeed = 0;
                else
                    ballYSpeed = 1;
            }
            else if (ballXPos >= SCREEN_WIDTH - 1)
            {
                ballXPos = SCREEN_WIDTH / 2;
                ballXSpeed = -1; // Reset speed when scoring
                ballYSpeed = 1;
                player1Score++;
            }
        }
        ballXPos += ballXSpeed;
    }
    else if (ballXSpeed < 0) // ball going left
    {
        if (ballXPos <= paddle1XPos + 1)
        {
            if (ballYPos >= paddle1YPos && ballYPos < paddle1YPos + PADDLE_HEIGHT)
            {
                ballXSpeed *= -1;

                // Adjust vertical speed based on where the ball hits the paddle
                int hitPosition = ballYPos - paddle1YPos;
                if (hitPosition < PADDLE_HEIGHT / 3)
                    ballYSpeed = -1;
                else if (hitPosition < 2 * PADDLE_HEIGHT / 3)
                    ballYSpeed = 0;
                else
                    ballYSpeed = 1;
            }
            else if (ballXPos <= 0)
            {
                ballXPos = SCREEN_WIDTH / 2;
                ballXSpeed = 1; // Reset speed when scoring
                ballYSpeed = 1;
                player2Score++;
            }
        }
        ballXPos += ballXSpeed;
    }
}

void printPlayerScores(wchar_t* screen)
{
    screen[SCORE_X_POSITION + SCORE_Y_POSITION * SCREEN_WIDTH] = L'0' + player1Score;
    screen[(SCREEN_WIDTH - SCORE_X_POSITION - 1) + SCORE_Y_POSITION * SCREEN_WIDTH] = L'0' + player2Score;
}

void checkWinningConditions(wchar_t* screen, bool& bGameOver)
{
    if (player1Score >= WINNING_SCORE)
    {
        //printPlayerOneWon(screen, L"Player 1 Wins!", 14);
        bGameOver = true;
    }
    else if (player2Score >= WINNING_SCORE)
    {
        //printPlayerTwoWon(screen, L"Player 2 Wins!", 14);
        bGameOver = true;
    }
}

void printPlayerWon(wchar_t* screen, const wchar_t* text, const unsigned int textLength)
{
    for (unsigned int i = 0; i < textLength; i++)
    {
        screen[(WINNER_X_POSITION + i + WINNER_Y_POSITION * SCREEN_WIDTH) - (textLength / 2)] = text[i];
    }
}

void resetGame(unsigned int& ballXPos, unsigned int& ballYPos, int& ballXSpeed, int& ballYSpeed)
{
    // Reset ball position
    ballXPos = SCREEN_WIDTH / 2;
    ballYPos = SCREEN_HEIGHT / 2;

    // Reset ball speed
    ballXSpeed = 1;
    ballYSpeed = 1;

    // Reset paddle positions
    paddle1YPos = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
    paddle2YPos = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
    // Reset scores
    player1Score = 0;
    player2Score = 0;
}