#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include "Adafruit_MPR121.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap1 = Adafruit_MPR121();
Adafruit_MPR121 cap2 = Adafruit_MPR121();
Adafruit_MPR121 cap3 = Adafruit_MPR121();

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, 3, NEO_GRB + NEO_KHZ800);

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched1 = 0;
uint16_t currtouched1 = 0;
uint16_t lasttouched2 = 0;
uint16_t currtouched2 = 0;
uint16_t lasttouched3 = 0;
uint16_t currtouched3 = 0;

void setup()
{
    Serial.begin(9600);
    strip.begin();
    strip.show();

    // while (!Serial)
    // { // needed to keep leonardo/micro from starting too fast!
    //     delay(10);
    // }

    Serial.println("Adafruit MPR121 Capacitive Touch sensor test");

    // Default address is 0x5A, if tied to 3.3V its 0x5B
    // If tied to SDA its 0x5C and if SCL then 0x5D
    if (!cap1.begin(0x5A))
    {
        Serial.println("MPR121 not found, check wiring?");
        while (1)
            ;
    }
    if (!cap2.begin(0x5B))
    {
        Serial.println("MPR121 not found, check wiring?");
        while (1)
            ;
    }
    if (!cap3.begin(0x5C))
    {
        Serial.println("MPR121 not found, check wiring?");
        while (1)
            ;
    }
    Serial.println("MPR121 found!");
}

void loop()
{
    // Get the currently touched pads
    currtouched1 = cap1.touched();
    currtouched2 = cap2.touched();
    currtouched3 = cap3.touched();

    char led;
    for (uint8_t i = 0; i < 12; i++)
    {
        // it if *is* touched and *wasnt* touched before, alert!
        if ((currtouched1 & _BV(i)) && !(lasttouched1 & _BV(i)))
        {
            led = i;
            strip.setPixelColor(led, 255, 0, 0);
            Serial.print(led);
            Serial.println(" touched on board 1");
        }
        // if it *was* touched and now *isnt*, alert!
        if (!(currtouched1 & _BV(i)) && (lasttouched1 & _BV(i)))
        {
            led = i;
            // strip.setPixelColor(led, 0, 0, 0);
            Serial.print(led);
            Serial.println(" released on board 1");
        }
    }
    for (uint8_t i = 0; i < 12; i++)
    {
        // it if *is* touched and *wasnt* touched before, alert!
        if ((currtouched2 & _BV(i)) && !(lasttouched2 & _BV(i)))
        {
            led = i + 12;
            strip.setPixelColor(led, 0, 0, 255);
            Serial.print(led);
            Serial.println(" touched on board 2");
        }
        // if it *was* touched and now *isnt*, alert!
        if (!(currtouched2 & _BV(i)) && (lasttouched2 & _BV(i)))
        {
            led = i + 12;
            // strip.setPixelColor(led, 0, 0, 0);
            Serial.print(led);
            Serial.println(" released on board 2");
        }
    }
    for (uint8_t i = 0; i < 12; i++)
    {
        // it if *is* touched and *wasnt* touched before, alert!
        if ((currtouched3 & _BV(i)) && !(lasttouched3 & _BV(i)))
        {
            led = i + 24;
            strip.setPixelColor(led, 0, 255, 0);
            Serial.print(led);
            Serial.println(" touched on board 3");
        }
        // if it *was* touched and now *isnt*, alert!
        if (!(currtouched3 & _BV(i)) && (lasttouched3 & _BV(i)))
        {
            led = i + 24;
            // strip.setPixelColor(led, 0, 0, 0);
            Serial.print(led);
            Serial.println(" released on board 3");
        }
    }
    strip.show();

    // reset our state
    lasttouched1 = currtouched1;
    lasttouched2 = currtouched2;
    lasttouched3 = currtouched3;

    // comment out this line for detailed data from the sensor!
    return;

    // // debugging info, what
    // Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x");
    // Serial.println(cap.touched(), HEX);
    // Serial.print("Filt: ");
    // for (uint8_t i = 0; i < 12; i++)
    // {
    //     Serial.print(cap.filteredData(i));
    //     Serial.print("\t");
    // }
    // Serial.println();
    // Serial.print("Base: ");
    // for (uint8_t i = 0; i < 12; i++)
    // {
    //     Serial.print(cap.baselineData(i));
    //     Serial.print("\t");
    // }
    // Serial.println();

    // // put a delay so it isn't overwhelming
    // delay(100);
}
