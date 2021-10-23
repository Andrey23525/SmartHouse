#include <RF24Network.h>
#include <RF24Network_config.h>
#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>
#include <RF24.h>

const int pinRadioCE = 9;
const int pinRadioCSN = 10;
const int delayBetweenRequests = 1000;

RF24 radio(pinRadioCE, pinRadioCSN);
RF24Network network(radio);

unsigned long lastTime = 0;
const uint16_t mainNode = 00;
const uint16_t thisNode = 01;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);
  radioInit();

  for (int i = 2; i < 5; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
  
  network.begin(90, thisNode);
}

int pin;

void loop() {
  network.update();
  
  while (network.available()) {

    RF24NetworkHeader receiverHeader;

    network.read(receiverHeader, &pin, sizeof(pin));

    pin++;

    Serial.println(pin);

    digitalWrite(pin, !digitalRead(pin));

  }

  lastTime = millis();

}

bool radioInit() {

  if (!radio.begin()) {
    return false;
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);


  return true;
}
