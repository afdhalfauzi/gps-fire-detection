#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial ser(18, 19);

struct ESPSERIAL
{
    bool flame1;
    bool flame2;
    int gas;
    String message;

    bool begin();
    String read();
    String splitString(String data, char separator, int index);
};

bool ESPSERIAL::begin()
{
    ser.begin(115200);
    ser.setTimeout(50);

    return 1;
}

String ESPSERIAL::read()
{
        ser.flush();
    if (ser.available())
    {
        // vTaskDelay(50);
        message = ser.readString();
        flame1 = ESPSERIAL::splitString(message, ';', 0) == "1" ? 1 : 0;
        flame2 = ESPSERIAL::splitString(message, ';', 1) == "1" ? 1 : 0;
        gas = ESPSERIAL::splitString(message, ';', 2).toInt();
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