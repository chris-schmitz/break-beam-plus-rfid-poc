#include <Wire.h>

void setup()
{
    Wire.begin(8);
    Wire.onReceive(receiveHandler);
    Serial.begin(9600); // * so we can output to the monitor
}

void loop()
{
    delay(100);
}

void receiveHandler(int howMany)
{
    while (1 < Wire.available())
    {
        char c = Wire.read();
        Serial.print(c);
    }
    int x = Wire.read();
    Serial.println(x);
}