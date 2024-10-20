#ifndef GAME_H
#define GAME_H

#include "GameControl.h"

void initialize();

void setUpGame();

void waiting();

void readDifficulty();

void startRound();

void gameOver();

void sleeping();

void turnOffLEDs();

void pulseRedLED();

bool isAnswerCorrect();

bool checkAnswerTimeout();

void reduceTimeFactor();

GameStatus getGameStatus();

#endif
