#define DISPLAY_SLEEP_AFTER 20000
#define AP_NAME "esp1"
#define AP_PASS NULL
// #define RADIO_NUMBER 0
#define RADIO_NUMBER 1

#include "./display.h"
#include "./radio.h"
#include "./server.h"

void setup()
{
  // ESP.wdtDisable(); // Power saving

  displaySetup();
  serverSetup();
  radioSetup();

  handleRoute("/", "<h1>You are connected!</h1>");

  displayPrint("Setup done");
}

void loop()
{
  displayWakeRoutine();

  server.handleClient();

  radio.stopListening();
  unsigned long start_time = micros();

  if (!radio.write(&start_time, sizeof(unsigned long))) {
    display.drawString(0, 0, "Failed");
  }

  radio.startListening();

  unsigned long started_waiting_at = micros();
  boolean timeout = false;

  while (!radio.available()) {
    yield();

    if (micros() - started_waiting_at > 200000) {
      timeout = true;
      break;
    }
  }

  if (timeout) {
    // displayPrint("TIMEOUT");
    return;
  }

  unsigned long got_time;
  radio.read(&got_time, sizeof(unsigned long));
  unsigned long end_time = micros();

  // displayPrint((String)(end_time - start_time));
}