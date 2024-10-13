#include "button.h"
#include <Arduino.h>

bool isPressed(int pin) {
  return digitalRead(pin) == HIGH;
}