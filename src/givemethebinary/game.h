#ifndef GAME_H
#define GAME_H

extern int ledPins[];
extern int buttonPins[];
extern int ledRedPin;
extern int potPin;

void setUpGame();

bool isAwake();

void start();

void pulseRedLED();

void setDifficulty();

#endif