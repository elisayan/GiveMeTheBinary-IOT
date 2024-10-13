#include "display.h"
#include <Arduino.h>

LCDDisplay() : lcd(12, 11, 5, 4, 3, 2) {
    // Costruttore che inizializza il display
}

void init() {
    lcd.begin(16, 2);
    showMessage("Welcome to GMB!");
}

void showMessage(char message) {
    lcd.clear();
    lcd.print(message);
}

void showScore(int score) {
    lcd.clear();
    lcd.print("Score: ");
    lcd.print(score);
}

void showRandomNumber(int number) {
    lcd.clear();
    lcd.print("Number: ");
    lcd.print(number);
}