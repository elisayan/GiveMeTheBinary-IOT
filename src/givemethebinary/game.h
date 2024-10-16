#ifndef GAME_H
#define GAME_H

bool isAwake(int buttonPin);

void start(int ledPins[]);

void pulseRedLED(int ledRedPin);

void setDifficulty(int ledRedPin, int potPin);

#endif