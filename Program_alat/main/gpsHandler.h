#include <Arduino.h>
#ifdef ESP32
#include <TinyGPSPlus.h>
#include <SoftwareSerial.h>

// The TinyGPSPlus object
TinyGPSPlus mygps;

int RXpin = 17, TXpin = 16;
int GPSBaud = 9600;

SoftwareSerial ss(RXpin, TXpin);

static void smartDelay(unsigned long ms);
struct GPS
{
    GPS(int _RXpin, int _TXpin)
    {
        RXpin = _RXpin;
        TXpin = _TXpin;
    }

    bool begin();
    bool isValid();
    String getLocation();
    String getLatitude();
    String getLongitude();
};

bool GPS::begin()
{
    ss.begin(GPSBaud);
    Serial.print(F("TinyGPSPlus library v. "));
    Serial.println(TinyGPSPlus::libraryVersion());
    Serial.println("GPS Ok");

    return 1;
}

bool GPS::isValid()
{
    return mygps.location.isValid();
    smartDelay(0);
}

String GPS::getLatitude()
{
    smartDelay(1000);
    return String(mygps.location.lat(), 6);
}

String GPS::getLongitude()
{
    smartDelay(1000);
    return String(mygps.location.lng(), 6);
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do
    {
        while (ss.available())
            mygps.encode(ss.read());
    } while (millis() - start < ms);
}
#endif