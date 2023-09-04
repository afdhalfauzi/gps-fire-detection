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
FLAME flame2(3);
GAS gas(A5);
SoftwareSerial serial(8, 9);
#endif

bool isFire = 0;
bool isGas = 0;
unsigned long millisThinger;

void sendSensorData();
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
  espSerial.flame1 == 1 || espSerial.flame2 == 1 ? isFire = 1 : isFire = 0;
  espSerial.gas > 700 ? isGas = 1 : isGas = 0;

  if (millis() - millisThinger >= 3000)
  {
    millisThinger = millis();
    String latitude = gps.getLatitude();
    String longitude = gps.getLongitude();
    thingerio.send(isFire, isGas, latitude, longitude);
    Serial.printf("fire=%i gas=%i latitude=%s longitude=%s\n", isFire, isGas, latitude.c_str(), longitude.c_str());
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
  Serial.println(payload);
  delay(100);
}
#endif