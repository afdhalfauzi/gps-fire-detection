#include <Arduino.h>
#ifdef ESP32
#include <ThingerESP32.h>
#define THINGER_SERIAL_DEBUG

// Thinger IO configuration
#define USERNAME "afdhalfauzi"
#define DEVICE_ID "deteksi_api"
#define DEVICE_CREDENTIAL "7J0HEZhH1dxWY$zS"

// WiFi configuration
#define SSID "BMZimages"
#define SSID_PASSWORD "bennamazarina"

ThingerESP32 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

struct THINGER
{
    bool fire = 0;
    bool gas;
    String location;

    bool begin();
    bool send(bool _fires, bool _gas, String _location);
};

bool THINGER::begin()
{
    thing.add_wifi(SSID, SSID_PASSWORD);

    thing["Sensor"] >> [&](pson &out)
    {
        out["fire"] = fire;
        out["gas"] = gas;
        out["location"] = location;
    };

    return 1;
}

bool THINGER::send(bool _fire, bool _gas, String _location)
{
    fire = _fire;
    gas = _gas;
    location = _location;
    if (fire || gas)
    {
        pson data;
        data["fire"] = fire;
        data["gas"] = gas;
        data["location"] = location;
        thing.call_endpoint("deteksi_api", data);
    }
    thing.handle();

    return 1;
}
#endif