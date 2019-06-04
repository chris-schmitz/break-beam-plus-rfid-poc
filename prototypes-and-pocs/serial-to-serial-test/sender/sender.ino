enum states
{
    idle = 0,
    start,
    complete
};

// char state[10] = "hello";
uint8_t currentState = idle;

#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

static const uint8_t PROGMEM
    smile_bmp[] =
        {B00111100,
         B01000010,
         B10100101,
         B10000001,
         B10100101,
         B10011001,
         B01000010,
         B00111100},
    neutral_bmp[] =
        {B00111100,
         B01000010,
         B10100101,
         B10000001,
         B10111101,
         B10000001,
         B01000010,
         B00111100},
    frown_bmp[] =
        {B00111100,
         B01000010,
         B10100101,
         B10000001,
         B10011001,
         B10100101,
         B01000010,
         B00111100};

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600);

    // while (!Serial)
    //     ;
    Serial.println("Serial ports open");

    matrix.begin(0x70);
}

void loop()
{
    Serial.println("Top of loop");
    if (currentState == idle)
    {
        currentState = start;
        matrix.clear();
        matrix.drawBitmap(0, 0, frown_bmp, 8, 8, LED_RED);
        matrix.writeDisplay();
        delay(500);
    }
    else if (currentState == start)
    {
        currentState = complete;
        matrix.clear();
        matrix.drawBitmap(0, 0, neutral_bmp, 8, 8, LED_YELLOW);
        matrix.writeDisplay();
        delay(500);
    }
    else if (currentState == complete)
    {
        currentState = idle;
        matrix.clear();
        matrix.drawBitmap(0, 0, smile_bmp, 8, 8, LED_GREEN);
        matrix.writeDisplay();
        delay(500);
    }
    Serial.print("Seinding state: ");
    Serial.println(currentState);
    Serial1.write(currentState);
    delay(1000);
}