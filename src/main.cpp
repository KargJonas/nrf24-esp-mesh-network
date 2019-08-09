#include "SSD1306Wire.h"
#include <Wire.h>
#define DISPLAY_SLEEP_AFTER 20000
#define WAKE_BUTTON 16

int wakeTime = 0;
bool displayOn = true;

SSD1306Wire display(0x3c, 4, 5, GEOMETRY_128_32);

void setup()
{
  pinMode(WAKE_BUTTON, INPUT);

  display.init();
  display.setFont(ArialMT_Plain_24);
  // ESP.wdtDisable(); // Power saving
}

void loop()
{
  bool wakeButtonPressed = digitalRead(WAKE_BUTTON);
  const int time = millis();

  if (displayOn) {
    if (time - wakeTime > DISPLAY_SLEEP_AFTER) {
      displayOn = false;
      display.displayOff();
    }
  } else if (wakeButtonPressed) {
    displayOn = true;
    wakeTime = time;
    display.displayOn();
  }

  display.clear();
  display.drawString(0, 0, wakeButtonPressed ? "TRUE" : "FALSE");
  display.display();
  delay(50);
}