#include "button.h"
#include <arduino.h>

bool isPressed(int pin)
{
    return digitalRead(pin) == HIGH;
}