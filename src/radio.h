#include "RF24.h"
#include <SPI.h>

RF24 radio(16, 15);

byte addresses[][6] = { "1Node", "2Node" };

void radioSetup()
{
  if (RADIO_NUMBER) {
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1, addresses[0]);
  } else {
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1, addresses[1]);
  }

  radio.startListening();
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
}