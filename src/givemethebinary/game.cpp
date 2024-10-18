#include <Arduino.h>
//#include <LiquidCrystal_I2C.h>
#include <EnableInterrupt.h>
#include <avr/sleep.h>
#include "Game.h"
#include "GameStatus.h"

GameStatus currentStatus = WAITING;

//LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);
int ledPins[] = { LED_1, LED_2, LED_3, LED_4 };
int buttonPins[] = { BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4 };
int ledRedPin = LED_S;
int potPin = POT;

int score;
int current;
int difficultyLevel;
int timeLimit;
int previousPotValue = -1;
int fadeAmount = 5;
int currIntensity = 0;
float factor;

unsigned long lastButtonPressTime = 0;
unsigned long sleepTimeout = 10000;

bool ledStates[LED_BUTTON_NUMBER] = { false, false, false, false };

void wakeUp() {
  lastButtonPressTime = millis();
  currentStatus = WAITING;
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

  for (int i = 0; i < LED_BUTTON_NUMBER; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT);
    digitalWrite(ledPins[i], LOW);
    enableInterrupt(buttonPins[i], wakeUp, RISING);
  }
  pinMode(ledRedPin, OUTPUT);

  lastButtonPressTime = millis();
  difficultyLevel = 1;
}

void waiting() {
  if (millis() - lastButtonPressTime >= sleepTimeout) {
    currentStatus = SLEEP_MODE;
  } else {
    analogWrite(ledRedPin, currIntensity);
    currIntensity = currIntensity + fadeAmount;
    if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount;
    }
    delay(50);

    readDifficulty();

    if (digitalRead(buttonPins[0]) == HIGH) {
      currentStatus = START_ROUND;
      //todo don't let it start

      for (int i = 0; i < LED_BUTTON_NUMBER; i++) {
        disableInterrupt(buttonPins[i]);
      }
      //lcd.clear();
      //lcd.print("Go!");
      Serial.println("Go!");

      //lcd.clear();
      //lcd.setCursor(0,1);
      //lcd.print("Difficulty level chosen: ");
      //lcd.setCursor(0,2);
      //lcd.print(difficultyLevel);
      Serial.print("Difficulty level chosen: ");
      Serial.println(difficultyLevel);

      score = 0;
      digitalWrite(ledRedPin, LOW);
    }
  }
}

void readDifficulty() {
  int potValue = analogRead(potPin);
  int newDifficultyLevel = map(potValue, 0, 1023, 1, 4);

  if (newDifficultyLevel != difficultyLevel) {
    difficultyLevel = newDifficultyLevel;
    Serial.print("You are choosing ");
    Serial.print(difficultyLevel);
    Serial.println(" of difficulty");

    factor = 1 - (difficultyLevel / 5.0);

    Serial.println(factor);
    previousPotValue = potValue;
  }
}

void startRound() {
  for (int i = 0; i < LED_BUTTON_NUMBER; i++) {
    if (digitalRead(buttonPins[i]) == HIGH) {
      if (digitalRead(ledPins[i]) == LOW) {
        digitalWrite(ledPins[i], HIGH);
      } else {
        digitalWrite(ledPins[i], LOW);
      }
    }
    delay(50);
  }
}

void userGuess() {
}

void gameOver() {
}

void sleeping() {
  //if (millis() - lastButtonPressTime >= sleepTimeout) {
  Serial.println("The game will enter power-down mode in 1 second. Press any button to prevent sleep.");
  delay(1000);
  Serial.flush();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  sleep_disable();
  Serial.println("The game has woken up from sleep mode.");
  //lastButtonPressTime = millis();
  //}
}

GameStatus getGameStatus() {
  return currentStatus;
}
