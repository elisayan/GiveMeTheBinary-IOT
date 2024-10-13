#include "score.h"
#include "Arduino.h"

static int score = 0;

void increase(){
    score++;
}

void reset(){
    score = 0;
}

int getScore(){
    return score;
}