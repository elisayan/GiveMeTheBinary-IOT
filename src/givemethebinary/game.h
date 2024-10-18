#ifndef GAME_H
#define GAME_H

#include "GameStatus.h"

void setUpGame();

void waiting();

void startRound();

void userGuess();

void readDifficulty();

void gameOver();

void sleeping();

GameStatus getGameStatus();

void pulseRedLED();

#endif
