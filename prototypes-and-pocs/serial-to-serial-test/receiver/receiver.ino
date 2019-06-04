#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel bar = Adafruit_NeoPixel(8, 2, NEO_GRB + NEO_KHZ800);

enum states
{
    idle = 0,
    start,
    complete
};

uint8_t currentState;

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);
    bar.begin();
    bar.show();
}

uint32_t color;

void loop()
{
    if (Serial1.available())
    {
        currentState = Serial1.read();

        Serial.print("Current state text: ");
        if (currentState == idle)
        {
            Serial.println("Idle");
            color = bar.Color(255, 0, 0);
        }
        else if (currentState == start)
        {
            Serial.println("Start");
            color = bar.Color(255, 255, 0);
        }
        else if (currentState == complete)
        {
            Serial.println("Complete!!");
            color = bar.Color(0, 255, 0);
        }
        else
        {

            Serial.println("unknown state :|");
        }

        for (uint8_t i = 0; i < 8; i++)
        {
            Serial.print("Color: ");
            Serial.println(color);
            bar.setPixelColor(i, color);
        }
        bar.show();
    }
    delay(1100);
    // delay(1000);
}