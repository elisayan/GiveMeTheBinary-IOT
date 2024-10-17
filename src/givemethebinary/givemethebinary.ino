/*
 * Assignment #1 - Give Me the Binary! (GMB)
 * Author: Elisa Yan
 */

#define LED_1 13
#define LED_2 12
#define LED_3 11
#define LED_4 10

#define BUTTON_1 2
#define BUTTON_2 3
#define BUTTON_3 4
#define BUTTON_4 5

#define LED_S 7
#define POT A0

//#include <LiquidCrystal_I2C.h>
#include <EnableInterrupt.h>
#include <avr/sleep.h>
#include "Game.h"

unsigned long lastButtonPressTime = 0;

int ledPins[] = { LED_1, LED_2, LED_3, LED_4 };
int buttonPins[] = { BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4 };
int ledRedPin = LED_S;
int potPin = POT;

const unsigned long sleepTimeout = 10000;

static bool gameStarted = false;

//LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4);

void wakeUp() {
}

void setup() {
  setUpGame();

  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT);
    enableInterrupt(buttonPins[i], wakeUp, RISING);
  }
  pinMode(ledRedPin, OUTPUT);

  lastButtonPressTime = millis();
}

void loop() {

  if (!gameStarted) {
    pulseRedLED();
    setDifficulty();

    if (!isAwake()) {
      if (millis() - lastButtonPressTime >= sleepTimeout) {
        Serial.println("The game are going in power down, you can sleep it by pressing any button");
        Serial.flush();
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_enable();
        sleep_mode();
        sleep_disable();
        Serial.println("wake up");
        lastButtonPressTime = millis();
      }
    } else {
      gameStarted = true;

      digitalWrite(ledRedPin, LOW);
    }
  } else {
    //lcd.clear();
    //lcd.print("Go!");
    Serial.println("Go!");

    start();
  }
}
