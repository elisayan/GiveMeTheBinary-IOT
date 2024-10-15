/*
 * Assignment #1 - Give Me the Binary! (GMB)
 * Author: Elisa Yan
 */

#include <LiquidCrystal.h>
#include "Game.h"

const int ledPins[] = { 13, A1, 6, 5 };
const int buttonPins[] = { A3, 2, 3, 4 };
const int potPin = A0;
const int ledRedPin = A1;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(ledRedPin, OUTPUT);
}

void loop() {
  static bool gameStarted = false;

  if (!gameStarted) {
    if (isAwake(buttonPins[0])) {
      gameStarted = true;
      start();
    }
  }
}
