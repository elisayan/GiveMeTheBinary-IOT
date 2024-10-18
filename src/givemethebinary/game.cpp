#include "game.h"
#include <Arduino.h>

int score;
int current;
int difficultyLevel;
float factor;
int timeLimit;
int previousPotValue = -1;
bool ledStates[4] = {false, false, false, false};

bool isAwake() {
  return digitalRead(buttonPins[0]) == HIGH;
}

void setUpGame() {
  Serial.begin(9600);

  //lcd.init();
  //lcd.backlight();
  //lcd.begin(16, 2);
  //lcd.print("Welcome to GMB!");
  //lcd.setCursor(0, 1);
  //lcd.print("Press B1 to Start");
  Serial.println("Welcome to GMB! Press B1 to Start");

  difficultyLevel = 1;
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void start() {
  //lcd.clear();
  //lcd.setCursor(0,1);
  //lcd.print("Difficulty level chosen: ");
  //lcd.setCursor(0,2);
  //lcd.print(difficultyLevel);

  Serial.print("Difficulty level chosen: ");
  Serial.println(difficultyLevel);

  score = 0;

  for (int i = 0; i < 4; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {  
      delay(50);                             
      while (digitalRead(buttonPins[i]) == HIGH);

      ledStates[i] = !ledStates[i];

      if (ledStates[i]) {
        digitalWrite(ledPins[i], HIGH);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
    }
  }
}

void pulseRedLED() {
  int brightness = (millis() % 2000) / 8;
  analogWrite(ledRedPin, brightness);
}

void setDifficulty() {
  int potValue = analogRead(potPin);
  int newDifficultyLevel = map(potValue, 0, 1023, 1, 4);

  if (newDifficultyLevel != difficultyLevel) {
    difficultyLevel = newDifficultyLevel;
    Serial.print("You are choosing ");
    Serial.print(difficultyLevel);
    Serial.println(" of difficulty");

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

    previousPotValue = potValue;
  }
}
