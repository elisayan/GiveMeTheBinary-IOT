/*
 * Assignment #1 - Give Me the Binary! (GMB)
 * Author: Elisa Yan
 */

#include "Game.h"
#include "GameStatus.h"

void setup() {
  setUpGame();
}

void loop() {
  const GameStatus gameStatus = getGameStatus();

  switch (gameStatus) {
    case STATUS_INITIALIZING:
      setUpGame();
      break;
    case STATUS_START_ROUND:
      startRound();
      break;
    case STATUS_AWAIT_USER_GUESS:
      userGuess();
      break;
    case STATUS_GAME_OVER:
      gameOver();
      break;
    case STATUS_SLEEP_MODE:
      sleeping();
      break;
    default:
      break;
  }
}
