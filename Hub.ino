#include <Thread.h>
#include <RF24Network.h>
#include <RF24Network_config.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define pinBluetoothTX 3
#define pinBluetoothRX 4
#define pinRadioCE  9
#define pinRadioCSN 10

RF24 radio(pinRadioCE, pinRadioCSN);
RF24Network network(radio);
SoftwareSerial bluetooth(pinBluetoothRX, pinBluetoothTX);

const uint16_t thisNode = 00;
const uint16_t anotherNode = 01;

int targetPin = 0;
bool newData = true;

Thread checkBluetoothThread = Thread();
Thread updateNetworkThread = Thread();

void setup() {
  Serial.begin(38400);
  bluetooth.begin(38400);

  while (!Serial) {}

  checkBluetoothThread.onRun(checkBluetooth);
  updateNetworkThread.onRun(updateNetwork);
  
  radioInit();
  network.begin(90, thisNode);
}

bool radioInit() {

  if (!radio.begin()) {
    return false;
  }

  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_LOW);

  return true;
}

void checkBluetooth() {
  if (bluetooth.available()) {
    String data = bluetooth.readString();

    if (data == "U") {
      network.begin(90, thisNode);
      return;
    }
    
    targetPin = data.toInt();
    newData = true;
    Serial.println(targetPin);

    
  }
}

void updateNetwork() {
  if (newData == true) {
    RF24NetworkHeader header(anotherNode);

    network.write(header, &targetPin, sizeof(targetPin));
    newData = false;
  }
}

void loop() {
  network.update();

  if (updateNetworkThread.shouldRun()) {
    updateNetworkThread.run();
  }

  if (checkBluetoothThread.shouldRun()) {
    checkBluetoothThread.run();
  }
}
