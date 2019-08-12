#include "SSD1306Wire.h"
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>

#define DISPLAY_SLEEP_AFTER 20000
#define WAKE_BUTTON 16
#define AP_NAME "esp1"
#define AP_PASS NULL

typedef std::function<void(void)> HandlerFunction;

SSD1306Wire display(0x3c, 4, 5, GEOMETRY_128_32);
ESP8266WebServer server(80);
String ip;

int wakeTime = 0;
bool displayOn = true;
bool wakeButtonPressed = false;

void handleRoute(
    String route,
    String response,
    String responseType = "text/html",
    uint8 responseStatus = 200)
{
  server.on(route, [response, responseType, responseStatus]() {
    server.send(responseStatus, responseType, response);
  });
}

struct Data {
  long runtime;
  String ip;
  byte connectedNodes[][6];
};

void
setup()
{
  // ESP.wdtDisable(); // Power saving
  pinMode(WAKE_BUTTON, INPUT);

  display.init();
  display.setFont(ArialMT_Plain_16);

  WiFi.softAP(AP_NAME, AP_PASS);

  handleRoute("/", "<h1>You are connected!</h1>");
  server.begin();

  delay(1000);
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
  }
}

void loop()
{
  displayWakeRoutine();

  display.clear();
  display.drawString(0, 0, ip);
  display.display();

  server.handleClient();
}