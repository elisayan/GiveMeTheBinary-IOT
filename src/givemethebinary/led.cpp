#include "led.h"
#include <arduino.h>

void turnOn(int pin)
{
    digitalWrite(pin, HIGH);
}

void turnOff(int pin)
{
    digitalWrite(pin, LOW);
}