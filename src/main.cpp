#include <Arduino.h>

#ifdef ESP32
#include <gpsHandler.h>
#include <thingerioHandler.h>
#include <serialHandler.h>
#include <LiquidCrystal_I2C.h>

THINGER thingerio;
GPS gps(17, 16);
ESPSERIAL espSerial;
LiquidCrystal_I2C lcd(0x27, 16, 2);

#else
#include <sensorHandler.h>
#include <SoftwareSerial.h>

#define BUZZER 7
#define LED 10
#define ALARM A2
#define GAS_TRESHOLD 200
FLAME flame1(2);
FLAME flame2(3);
FLAME flame3(4);
FLAME flame4(5);
FLAME flame5(6);
FLAME flame6(A0);
FLAME flame7(A1);
FLAME flame8(A3);
FLAME flame9(A4);
FLAME flame10(A6);
GAS gas(A5);
SoftwareSerial serial(8, 9);
#endif

bool isFire = 0;
bool isGas = 0;
String latitude, longitude;
unsigned long millisThinger = 0;

// custom char for lcd
uint8_t check[8] = {0x0, 0x1, 0x3, 0x16, 0x1c, 0x8, 0x0};
uint8_t cross[8] = {0x0, 0x1b, 0xe, 0x4, 0xe, 0x1b, 0x0};

bool fireDetect();
bool gasDetect();
void sendSensorData();
void display();
void thingerIO(void *parameter);
void setup()
{
  Serial.begin(115200);

#ifdef ESP32
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, cross); // load custom char
  lcd.createChar(1, check);
  gps.begin();
  espSerial.begin();
  xTaskCreate(thingerIO, "thingerIO task", 1024 * 5, NULL, 1, NULL);
#else
  flame1.begin();
  flame2.begin();
  flame3.begin();
  flame4.begin();
  flame5.begin();
  flame6.begin();
  flame7.begin();
  flame8.begin();
  flame9.begin();
  flame10.begin();
  gas.begin();
  serial.begin(115200);
  serial.setTimeout(50);
  pinMode(BUZZER, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(ALARM, OUTPUT);
#endif
}

void loop()
{
#ifdef ESP32
  espSerial.read();
  isFire = espSerial.flame;
  isGas = espSerial.gas;

  display();
#else
  sendSensorData();
#endif
}

void display()
{
  lcd.setCursor(0, 0);
  lcd.print("WiFi[");
  lcd.setCursor(9, 0);
  lcd.print("GPS[");
  lcd.setCursor(4, 1);
  lcd.print("Alarm[");

  lcd.setCursor(5, 0);
  WiFi.status() == WL_CONNECTED ? lcd.write(1) : lcd.write(0); // WiFi Indicator
  lcd.print("]");
  lcd.setCursor(13, 0);
  gps.isValid() ? lcd.write(1) : lcd.write(0); // GPS Indicator
  lcd.print("]");
  lcd.setCursor(10, 1);
  isFire || isGas ? lcd.write(1) : lcd.write(0); // Alarm Indicator
  lcd.print("]");
}

void thingerIO(void *parameter)
{
  thingerio.begin();
  for(;;)
  {
    latitude = gps.getLatitude();
    longitude = gps.getLongitude();
    thingerio.send(isFire, isGas, latitude, longitude);
    Serial.printf("fire=%i gas=%i latitude=%s longitude=%s\n", isFire, isGas, latitude.c_str(), longitude.c_str());
    vTaskDelay(3000);
  }
}

#ifndef ESP32
void sendSensorData()
{
  char payload[6];
  sprintf(payload, "%i;%i", fireDetect(), gasDetect());
  if (fireDetect() || gasDetect())
  {
    digitalWrite(LED, LOW);   // active low
    digitalWrite(ALARM, LOW); // active low
    digitalWrite(BUZZER, HIGH);
  }
  else
  {
    digitalWrite(LED, HIGH);
    digitalWrite(ALARM, HIGH);
    digitalWrite(BUZZER, LOW);
  }
  serial.write(payload); // kirim melalui serial
  Serial.println(payload);
  delay(100);
}

bool fireDetect()
{
  bool flameRead[10] = {flame1.read(), flame2.read(), flame3.read(), flame4.read(), flame5.read(), flame6.read(), flame7.read(), flame8.read(), flame9.read(), flame10.read()};
  for (int i = 0; i < 9; i++) // check if any flame detected
  {
    if (flameRead[i] == 1)
      return 1;
    Serial.print(flameRead[i]);
  }
  Serial.println();
  return 0;
}

bool gasDetect()
{
  if (gas.read() > GAS_TRESHOLD)
    return 1;
  else
    return 0;
}
#endif