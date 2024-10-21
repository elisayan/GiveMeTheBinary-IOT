#ifndef GAME_CONTROL_H
#define GAME_CONTROL_H

#define TIME_OUT 10000
#define ANSWER_TIME_LIMIT 15000

#define LED_BUTTON_NUMBER 4
#define BIT_NUMBER 4

#define LED_1 13
#define LED_2 12
#define LED_3 11
#define LED_4 10

#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 4
#define BUTTON_4 5

#define LED_S 6
#define POT A0

typedef enum GameStatus {
    WAITING,
    START_ROUND,
    GAME_OVER, 
    SLEEP_MODE
};

#endif
