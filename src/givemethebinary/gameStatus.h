#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#define LED_BUTTON_NUMBER 4

#define LED_1 13
#define LED_2 12
#define LED_3 11
#define LED_4 10

#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 4
#define BUTTON_4 5

#define LED_S 7
#define POT A0

typedef enum GameStatus {
    STATUS_INITIALIZING,
    STATUS_START_ROUND,
    STATUS_AWAIT_USER_GUESS,
    STATUS_GAME_OVER, 
    STATUS_SLEEP_MODE
} GameStatus;

#endif
