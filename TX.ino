#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//NRF24L01
//9 CE, 10 CSN, 11 MISO, 12 MOSI, 13 SCK
//LCD
//7 6 5 4 3
//JOYSTICK
//A0, A1, 2
//A4, A5, 8
//AUDIO
//0, 1

RF24 radio(9, 10);
const byte addresses[][6] = { "00001", "00002", "00003", "00004" };
char receivedData[32] = "";
int joystick[4];
int stat[2];

const int JOYSTICK_X_PIN = A0;
const int JOYSTICK_BTN1_PIN = 0;
const int JOYSTICK_Y_PIN = A4;
const int JOYSTICK_BTN2_PIN = 2;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1, addresses[0]);
  radio.setPALevel(RF24_PA_MAX);
  pinMode(JOYSTICK_BTN1_PIN, INPUT_PULLUP);
  pinMode(JOYSTICK_BTN2_PIN, INPUT_PULLUP);
}

void loop() {
  radio.stopListening();

  joystick[0] = map(analogRead(JOYSTICK_X_PIN), 0, 1024, -255, 255);
  joystick[0] = (joystick[0] < -23 || joystick[0] > 23) ? joystick[0] : 0;
  joystick[1] = !digitalRead(JOYSTICK_BTN1_PIN);

  joystick[2] = map(analogRead(JOYSTICK_Y_PIN), 0, 1024, -255, 255);
  joystick[2] = (joystick[2] < -23 || joystick[2] > 23) ? joystick[2] : 0;
  joystick[3] = !digitalRead(JOYSTICK_BTN2_PIN);

  radio.write(joystick, sizeof(joystick));

  Serial.print(joystick[0]);
  Serial.print(" ");
  Serial.println(joystick[1]);
  Serial.print(joystick[2]);
  Serial.print(" ");
  Serial.println(joystick[3]);

  radio.startListening();
  delay(50);

  if (radio.available()) {
    radio.read(stat, sizeof(stat));
    radio.read(&receivedData, sizeof(receivedData));
    Serial.print("PWR: ");
    Serial.print(stat[0]);
    Serial.print("/5 HP: ");
    Serial.print(stat[1]);
    Serial.println("/5");
  }

  delay(300);
}
