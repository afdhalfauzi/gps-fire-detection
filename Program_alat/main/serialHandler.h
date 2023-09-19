#include <Arduino.h>

struct ESPSERIAL
{
    bool flame;
    bool gas;
    String message;

    bool begin();
    String read();
    String splitString(String data, char separator, int index);
};

bool ESPSERIAL::begin()
{
    Serial2.begin(115200, SERIAL_8N1, 18, 19);
    Serial2.setTimeout(50);

    return 1;
}

String ESPSERIAL::read()
{
    if (Serial2.available())
    {
        message = Serial2.readString();
        flame = splitString(message, ';', 0) == "1" ? 1 : 0; // string to bool
        gas = splitString(message, ';', 1) == "1" ? 1 : 0;   // string to bool
        Serial.println(message);
    }
    return message;
}

String ESPSERIAL::splitString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = {0, -1};
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++)
    {
        if (data.charAt(i) == separator || i == maxIndex)
        {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i + 1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}