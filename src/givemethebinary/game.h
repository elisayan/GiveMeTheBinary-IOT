#ifndef GAME_H
#define GAME_H

#include "GameStatus.h"

void setUpGame();

void startRound();

void userGuess();

void setDifficulty();

void gameOver();

void sleeping();

GameStatus getGameStatus();

void pulseRedLED();

#endif
