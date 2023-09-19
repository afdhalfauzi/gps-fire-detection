#include <Arduino.h>
#ifdef ESP32
#include <ThingerESP32.h>
#define THINGER_SERIAL_DEBUG

// Thinger IO configuration
#define USERNAME "gpsfiredetection"
#define DEVICE_ID "deteksi_api"
#define DEVICE_CREDENTIAL "IE6GuPu-pnRAn-hK"

// WiFi configuration
#define SSID "BMZimages"
#define SSID_PASSWORD "bennamazarina"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

struct THINGER
{
    bool fire;
    bool gas;
    String fireState;
    String gasState;
    String latitude;
    String longitude;

    bool begin();
    bool send(bool _fire, bool _gas, String _latitude, String _longitude);
};

bool THINGER::begin()
{
    thing.add_wifi(SSID, SSID_PASSWORD);

    thing["Sensor"] >> [&](pson &out)
    {
        out["fire"] = fireState;
        out["gas"] = gasState;
        out["latitude"] = latitude;
        out["longitude"] = longitude;
    };

    return 1;
}

bool THINGER::send(bool _fire, bool _gas, String _latitude, String _longitude)
{
    fire = _fire;
    gas = _gas;
    latitude = _latitude;
    longitude = _longitude;
    fire ? fireState = "Api Terdeteksi" : fireState = "Aman";
    gas ? gasState = "Gas Terdeteksi" : gasState = "Aman";
    if (fire || gas) // Send email notification
    {
        pson data;
        data["fire"] = fireState;
        data["gas"] = gasState;
        data["latitude"] = latitude;
        data["longitude"] = longitude;
        thing.call_endpoint("deteksi_api", data);
    }
    thing.handle();

    return 1;
}
#endif