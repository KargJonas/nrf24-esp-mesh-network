#include "SSD1306Wire.h"
#define WAKE_BUTTON 16

SSD1306Wire display(0x3c, 4, 5, GEOMETRY_128_32);

int wakeTime = 0;
bool displayOn = true;
bool wakeButtonPressed = false;

void displaySetup()
{
  pinMode(WAKE_BUTTON, INPUT);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_16);
}

void displayPrint(String text)
{
  display.clear();
  display.drawString(0, 0, text);

  if (displayOn) {
    display.display();
  }
}

void displayWakeRoutine()
{
  wakeButtonPressed = digitalRead(WAKE_BUTTON);

  if (displayOn) {
    if (millis() - wakeTime > DISPLAY_SLEEP_AFTER) {
      displayOn = false;
      display.displayOff();
    }
  } else if (wakeButtonPressed) {
    displayOn = true;
    wakeTime = millis();
    display.displayOn();
    display.display();
  }

  displayPrint((String)wakeButtonPressed);
}