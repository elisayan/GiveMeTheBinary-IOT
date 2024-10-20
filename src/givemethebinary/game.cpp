#include <Arduino.h>
//#include <LiquidCrystal_I2C.h>
#include <EnableInterrupt.h>
#include <avr/sleep.h>
#include "Game.h"
#include "GameControl.h"

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
float factor = 1.0;
int binaryNumber[BIT_NUMBER + 1];

unsigned long lastButtonPressTime = 0.0;
unsigned long generateNumberTime = 0.0;
unsigned long timeout = 10000.0;
unsigned long timeAnswer = 15000.0;

bool ledStates[LED_BUTTON_NUMBER] = { false, false, false, false };
int activeLEDs[LED_BUTTON_NUMBER] = { 0, 0, 0, 0 };

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
  turnOffLEDs();
  bitNumber = BIT_NUMBER;
  number = random(0, 16);

  //lcd.clear();
  //lcd.setCursor(0,1);
  //lcd.print("Number: ");
  //lcd.print(number);
  Serial.print("Number: ");
  Serial.println(number);

  uint8_t i = 0;
  while (bitNumber--) {
    binaryNumber[i++] = bitRead(number, bitNumber) + 0;
  }
  binaryNumber[i] = 0;
  for (int i = 0; i < BIT_NUMBER; i++) {
    Serial.print(binaryNumber[i]);
  }
  Serial.println();
  generateNumberTime = millis();
}

void setUpGame() {
  //lcd.init();
  //lcd.backlight();
  //lcd.begin(16, 2);
  //lcd.print("Welcome to GMB!");
  //lcd.setCursor(0, 1);
  //lcd.print("Press B1 to Start");
  Serial.println("Welcome to GMB! Press B1 to Start");
  turnOffLEDs();

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
    pulseRedLED();
    readDifficulty();
    delay(150);

    if (digitalRead(buttonPins[0]) == HIGH) {
      currentStatus = START_ROUND;

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
        activeLEDs[i] = 1;
      } else {
        digitalWrite(ledPins[i], LOW);
        activeLEDs[i] = 0;
      }
      delay(100);
    }
  }
  // Serial.print("AFTER: ");
  // for (int i = 0; i < BIT_NUMBER; i++) {
  //   Serial.print(activeLEDs[i]);
  // }
  // Serial.println();


  if (isAnswerCorrect()) {
    score++;
    //lcd.clear();
    //lcd.setCursor(0,1);
    //lcd.print("GOOD! Score: ");
    //lcd.setCursor(0,2);
    //lcd.print(score);
    Serial.print("GOOD! Score: ");
    Serial.println(score);
    Serial.println();
    reduceTimeFactor();
    newRound();
  }

  if (checkAnswerTimeout()) {
    Serial.println("Wrong answer or time's up!");
    Serial.print("The correct answer is: ");
    for (int i = 0; i < BIT_NUMBER; i++) {
      Serial.print(binaryNumber[i]);
    }
    Serial.println();
    Serial.print("YOUR ANSWER: ");
    for (int i = 0; i < BIT_NUMBER; i++) {
      Serial.print(activeLEDs[i]);
    }
    Serial.println();
    currentStatus = GAME_OVER;
  }
  delay(100);
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
  Serial.println("The game will enter power-down mode in 1 second. Press any button to awoken.");
  delay(1000);
  Serial.flush();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode();

  sleep_disable();
  Serial.println("The game has woken up from sleep mode.");
}

void turnOffLEDs() {
  for (int i = 0; i < LED_BUTTON_NUMBER; i++) {
    digitalWrite(ledPins[i], LOW);
    activeLEDs[i] = 0;
  }
}

void pulseRedLED() {
  analogWrite(ledRedPin, currIntensity);
  currIntensity = currIntensity + fadeAmount;
  if (currIntensity == 0 || currIntensity == 255) {
    fadeAmount = -fadeAmount;
  }
}

bool isAnswerCorrect() {
  for (int i = 0; i < BIT_NUMBER; i++) {
    if (activeLEDs[i] != binaryNumber[i]) {
      return false;
    }
  }
  return true;
}

bool checkAnswerTimeout() {
  return millis() - generateNumberTime >= timeAnswer * factor;
}

void reduceTimeFactor() {
  factor *= 0.9;

  if (factor < 0.2) {
    factor = 0.2;
  }
}

GameStatus getGameStatus() {
  return currentStatus;
}
