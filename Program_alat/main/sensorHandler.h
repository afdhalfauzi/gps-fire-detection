#include <Arduino.h>
struct FLAME
{
    int pin;

    FLAME(int _pin)
    {
        pin = _pin;
    } // constructor

    bool begin();
    int read();
};

bool FLAME::begin()
{
    pinMode(pin, INPUT);
    return 1;
}

int FLAME::read()
{
    return digitalRead(pin);
}

struct GAS
{
    int pin;
    GAS(int _pin)
    {
        pin = _pin;
    } // constructor

    bool begin();
    int read();
};

bool GAS::begin()
{
    pinMode(pin, INPUT);
    return 1;
}

int GAS::read()
{
    return analogRead(pin);
}
