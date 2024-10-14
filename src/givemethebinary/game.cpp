#include "game.h"
#include <Arduino.h>

void start(Display lcd, Led led) {
  lcd.showMessage("Go!");
  score = 0;
}