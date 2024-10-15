#include "game.h"
#include <Arduino.h>

int score;

bool isAwake(int buttonPin){
  return digitalRead(buttonPin) == HIGH;
}

void start() {
  
  score = 0;
}