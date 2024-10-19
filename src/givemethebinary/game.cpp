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
int number;
int current;
int difficultyLevel;
int timeLimit;
int previousPotValue = -1;
int fadeAmount = 5;
int currIntensity = 0;
int bitNumber = BIT_NUMBER;
float factor;
int binaryNumber[BIT_NUMBER + 1];

unsigned long lastButtonPressTime = 0.0;
unsigned long generateNumberTime = 0.0;
unsigned long timeout = 10000.0;
unsigned long timeAnswer = 15000.0;

bool ledStates[LED_BUTTON_NUMBER] = { false, false, false, false };
int ledPressed[LED_BUTTON_NUMBER] = { 0, 0, 0, 0 };

void wakeUp() {
  lastButtonPressTime = millis();
  currentStatus = WAITING;
}

void initialize() {
  Serial.begin(9600);
  randomSeed(analogRead(A1));

  for (int i = 0; i < LED_BUTTON_NUMBER; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT);
  }
  pinMode(ledRedPin, OUTPUT);
}

void newRound() {
  bitNumber = BIT_NUMBER;
  offLed();

  number = random(0, 16);

  Serial.print("Number: ");
  Serial.println(number);

  uint8_t i = 0;
  while (bitNumber--) {
    binaryNumber[i++] = bitRead(number, bitNumber) + 0;
  }
  binaryNumber[i] = 0;
  for (int i = 0; i < BIT_NUMBER; i++) {
    Serial.println(binaryNumber[i]);
  }
  generateNumberTime = millis();
}

void offLed() {
  for (int i = 0; i < LED_BUTTON_NUMBER; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

void setUpGame() {
  //lcd.init();
  //lcd.backlight();
  //lcd.begin(16, 2);
  //lcd.print("Welcome to GMB!");
  //lcd.setCursor(0, 1);
  //lcd.print("Press B1 to Start");
  Serial.println("Welcome to GMB! Press B1 to Start");
  offLed();

  for (int i = 0; i < LED_BUTTON_NUMBER; i++) {
    enableInterrupt(buttonPins[i], wakeUp, RISING);
  }

  lastButtonPressTime = millis();
  difficultyLevel = 1;
}

void waiting() {
  if (millis() - lastButtonPressTime >= timeout) {
    currentStatus = SLEEP_MODE;
  } else {
    analogWrite(ledRedPin, currIntensity);
    currIntensity = currIntensity + fadeAmount;
    if (currIntensity == 0 || currIntensity == 255) {
      fadeAmount = -fadeAmount;
    }

    readDifficulty();
    delay(150);

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

      newRound();

      score = 0;
      digitalWrite(ledRedPin, LOW);
    }
  }
  delay(50);
}

void readDifficulty() {
  int potValue = analogRead(potPin);
  int newDifficultyLevel = map(potValue, 0, 1023, 1, 4);

  if (newDifficultyLevel != difficultyLevel) {
    difficultyLevel = newDifficultyLevel;
    Serial.print("You are choosing ");
    Serial.print(difficultyLevel);
    Serial.println(" level of difficulty");

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
        ledPressed[i] = 1;
      } else {
        digitalWrite(ledPins[i], LOW);
        ledPressed[i] = 0;
      }
    }
    delay(50);
  }

  if (checkAnswer()) {
    if (!timeAnswerOut()) {
      score++;
      //lcd.clear();
      //lcd.setCursor(0,1);
      //lcd.print("GOOD! Score: ");
      //lcd.setCursor(0,2);
      //lcd.print(score);
      Serial.print("GOOD! Score: ");
      Serial.println(score);
      reduceTimeFactor();
      newRound();
    } else {
      Serial.println("Time's up! Moving to game over.");
      currentStatus = GAME_OVER;
    }
  } else if (timeAnswerOut()) {
    Serial.println("Wrong answer or time's up!");
    currentStatus = GAME_OVER;
  }

  delay(25);
}

bool timeAnswerOut() {
  return millis() - generateNumberTime >= timeAnswer * factor;
}

bool checkAnswer() {
  for (int i = 0; i < BIT_NUMBER; i++) {
    if (ledPressed[i] != binaryNumber[i]) {
      return false;
    }
  }
  return true;
}

void gameOver() {
  digitalWrite(ledRedPin, HIGH);
  Serial.print("Game Over - Final Score: ");
  Serial.println(score);
  delay(timeout);
  setUpGame();
  currentStatus = WAITING;
}

void sleeping() {
  Serial.println("The game will enter power-down mode in 1 second. Press any button to prevent sleep.");
  delay(1000);
  Serial.flush();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  sleep_disable();
  Serial.println("The game has woken up from sleep mode.");
}

GameStatus getGameStatus() {
  return currentStatus;
}

void reduceTimeFactor() {
  factor *= 0.9;

  if (factor < 0.2) {
    factor = 0.2;
  }
}
