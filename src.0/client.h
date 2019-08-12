#include "SSD1306Wire.h"
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>

SSD1306Wire display(0x3c, 4, 5, GEOMETRY_128_32);

bool radioNumber = 1;
RF24 radio(D4, D8);

byte addresses[][6] = { "1Node", "2Node" };
bool role = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  display.init();
  display.setFont(ArialMT_Plain_24);

  radio.begin();
  radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if (radioNumber) {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
  } else {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  }

  // Start the radio listening for data
  radio.startListening();
}
void loop()
{
  display.clear();
  display.drawString(0, 0, role == 1 ? "Ping Out" : "Pong Back");
  display.display();

  /****************** Ping Out Role ***************************/
  if (role == 1) {
    radio.stopListening(); // First, stop listening so we can talk.

    Serial.println(F("Now sending"));
    unsigned long start_time = micros(); // Take the time, and send it.  This will block until complete
    if (!radio.write(&start_time, sizeof(unsigned long))) {
      Serial.println(F("failed"));
    }

    radio.startListening(); // Now, continue listening

    unsigned long started_waiting_at = micros(); // Set up a timeout period, get the current microseconds
    boolean timeout = false; // Set up a variable to indicate if a response was received or not

    while (!radio.available()) { // While nothing is received
      if (micros() - started_waiting_at > 200000) { // If waited longer than 200ms, indicate timeout and exit while loop
        timeout = true;
        break;
      }
    }

    if (timeout) { // Describe the results
      Serial.println(F("Failed, response timed out."));
    } else {
      unsigned long got_time; // Grab the response, compare, and send to debugging spew
      radio.read(&got_time, sizeof(unsigned long));
      unsigned long end_time = micros();

      // Spew it
      Serial.print(F("Sent "));
      Serial.print(start_time);
      Serial.print(F(", Got response "));
      Serial.print(got_time);
      Serial.print(F(", Round-trip delay "));
      Serial.print(end_time - start_time);
      Serial.println(F(" microseconds"));
    }
    // Try again 1s later
    delay(1000);
  }
  /****************** Pong Back Role ***************************/
  if (role == 0) {
    unsigned long got_time;

    if (radio.available()) {
      // Variable for the received timestamp
      while (radio.available()) { // While there is data ready
        radio.read(&got_time, sizeof(unsigned long)); // Get the payload
      }

      radio.stopListening(); // First, stop listening so we can talk
      radio.write(&got_time, sizeof(unsigned long)); // Send the final one back.
      radio.startListening(); // Now, resume listening so we catch the next packets.
      Serial.print(F("Sent response "));
      Serial.println(got_time);
    }
  }
  /****************** Change Roles via Serial Commands ***************************/
  if (Serial.available()) {
    char c = toupper(Serial.read());
    if (c == 'T' && role == 0) {
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1; // Become the primary transmitter (ping out)

    } else if (c == 'R' && role == 1) {
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));
      role = 0; // Become the primary receiver (pong back)
      radio.startListening();
    }
  }
}