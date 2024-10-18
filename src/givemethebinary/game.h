#ifndef GAME_H
#define GAME_H

#include "GameStatus.h"

void initialize();

void offLed();

void setUpGame();

void waiting();

void startRound();

void readDifficulty();

void gameOver();

void sleeping();

GameStatus getGameStatus();

void pulseRedLED();

bool checkAnswer();

bool timeAnswerOut();

#endif
