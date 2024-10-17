#include "game.h"
#include <Arduino.h>

int score;
int current;
int difficultyLevel;
float factor;
int timeLimit;

bool isAwake(int buttonPin) {
  return digitalRead(buttonPin) == HIGH;
}

void start(int ledPins[], int buttonPins[]) {
  score = 0;

  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void pulseRedLED(int ledRedPin) {
  int brightness = (millis() % 2000) / 8;
  analogWrite(ledRedPin, brightness);
}

void setDifficulty(int ledRedPin, int potPin) {
  int potValue = analogRead(potPin);

  difficultyLevel = map(potValue, 0, 1023, 1, 4);

  Serial.print("Difficulty Level: ");
  Serial.println(difficultyLevel);

  switch (difficultyLevel) {
    case 1:
      factor = 1.0;
      break;
    case 2:
      factor = 0.8;
      break;
    case 3:
      factor = 0.6;
      break;
    case 4:
      factor = 0.4;
      break;
  }
}