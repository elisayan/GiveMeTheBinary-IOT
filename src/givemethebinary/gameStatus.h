#ifndef GAME_STATUS_H
#define GAME_STATUS_H

typedef enum GameStatus {
    STATUS_INITIALIZING,
    STATUS_SELECT_DIFFICULTY,
    STATUS_START_ROUND,
    STATUS_AWAIT_USER_GUESS,
    STATUS_GAME_OVER, 
    STATUS_SLEEP_MODE
} GameStatus;

#endif
