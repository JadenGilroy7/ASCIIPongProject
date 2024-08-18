#pragma once

void drawBlankField(wchar_t* screen);
void drawVerticalBars(wchar_t* screen);
void drawHorizontalBorders(wchar_t* screen);
void handlePaddleOneMovement(wchar_t* screen, const bool bKey[4]);
void handlePaddleTwoMovement(wchar_t* screen, const bool bKey[4]);
void drawPaddleOne(wchar_t* screen);
void drawPaddleTwo(wchar_t* screen);
void repositionBallVertically(unsigned int& ballYPos, int& ballYSpeed);
void repositionBallHorizontally(const unsigned int ballYPos, unsigned int& ballXPos, int& ballXSpeed, int& ballYSpeed);
void printPlayerScores(wchar_t* screen);
void checkWinningConditions(wchar_t* screen, bool& bGameOver);
void printPlayerWon(wchar_t* screen, const wchar_t* text, const unsigned int textLength);
void resetGame(unsigned int& ballXPos, unsigned int& ballYPos, int& ballXSpeed, int& ballYSpeed);