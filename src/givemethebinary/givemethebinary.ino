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

const unsigned long idleTimeout = 10000;

static bool gameStarted;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.print("Welcome to GMB!");
  lcd.setCursor(0, 1);
  lcd.print("Press B1 to Start");

  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(ledRedPin, OUTPUT);

  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  int newValue = analogRead(potPin);

  gameStarted = false;

  if (!gameStarted) {

    pulseRedLED(ledRedPin);
    setDifficulty(ledRedPin, potPin);

    if (isAwake(buttonPins[0])) {
      gameStarted = true;
      lcd.clear();
      lcd.print("Go!");
      digitalWrite(ledRedPin, LOW);
      start(ledPins);
      Serial.println(newValue);
    }
  }
}
