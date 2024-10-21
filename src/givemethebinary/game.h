#ifndef GAME_H
#define GAME_H

#include "gameControl.h"

void initialize();

void setUpGame();

void waiting();

void startRound();

void gameOver();

void sleeping();

GameStatus getGameStatus();

#endif
