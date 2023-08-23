#include <Arduino.h>

#ifdef ESP32
#include <gpsHandler.h>
#include <thingerioHandler.h>
#include <serialHandler.h>

THINGER thingerio;
GPS gps(17, 16);
ESPSERIAL espSerial;
#else
#include <sensorHandler.h>
#include <SoftwareSerial.h>

FLAME flame1(2);
FLAME flame2(7);
GAS gas(A5);
SoftwareSerial serial(8, 9);
#endif

bool isFire = 0;
bool isGas = 0;
unsigned long millisThinger;

void fireDetect(bool *state);
void gasDetect(bool *state, int tresshold);

void setup()
{
  Serial.begin(115200);

#ifdef ESP32
  gps.begin();
  thingerio.begin();
  espSerial.begin();
#else
  flame1.begin();
  flame2.begin();
  gas.begin();
  serial.begin(115200);
  serial.setTimeout(50);
  void sendSensorData();
#endif
}

void loop()
{
#ifdef ESP32
  espSerial.read();
  // espSerial.flame1 == 1 || espSerial.flame2 == 1 ? isFire = 1 : isFire = 0;
  fireDetect(&isFire);
  // espSerial.gas > 700 ? isGas = 1 : isGas = 0;
  gasDetect(&isGas, 700);

  if (millis() - millisThinger >= 3000)
  {
    millisThinger = millis();
    thingerio.send(isFire, isGas, gps.getLocation());
    Serial.printf("f1=%i f2=%i gas=%i received=%s send=%i,%i\n", espSerial.flame1, espSerial.flame2, espSerial.gas, espSerial.message.c_str(), isFire, isGas);
  }

#else
  sendSensorData();
#endif
}

#ifndef ESP32
void sendSensorData()
{
  char payload[6];
  sprintf(payload, "%i;%i;%i;", flame1.read(), flame2.read(), gas.read());
  serial.write(payload);
  delay(1000);
}
#endif

void fireDetect(bool *state)
{
  espSerial.flame1 == 1 || espSerial.flame2 == 1 ? *state = true : *state = false;
}

void gasDetect(bool *state, int tresshold)
{
  espSerial.gas > tresshold ? *state = true : *state = false;
}