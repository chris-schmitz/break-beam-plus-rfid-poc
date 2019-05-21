#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define PIN 1

// #define TOTAL_LEDS 8
#define TOTAL_LEDS 22

Adafruit_NeoPixel strip = Adafruit_NeoPixel(TOTAL_LEDS, PIN, NEO_GRB + NEO_KHZ800);

enum gameStates
{
    IDLE = 0,
    START,
    PROCESSING,
    COMPLETE
};

int currentGameState = IDLE;

void setup()
{
    // * Start up our serial ports
    Serial.begin(9600);
    // * Leaving this in here b/c it's nice to be able to pause for troubleshooting purposes
    // while (!Serial)
    //     ;

    // * Starting up i2c
    Wire.begin(8);
    Wire.onReceive(handleMessageFromLeader);

    // * Start up our led strip
    strip.begin();
    strip.show();
    ready();
}

void ready()
{
    colorWipe(strip.Color(255, 0, 0), 25);
    colorWipe(strip.Color(0, 255, 0), 25);
    colorWipe(strip.Color(0, 0, 255), 25);
    colorWipe(strip.Color(0, 0, 0), 25);
    Serial.println("ready");
}

void handleMessageFromLeader(int message)
{
    Serial.println("Got message from leader");
    Serial.print("bytes: ");
    Serial.println(Wire.available());

    if (Wire.available() == 0)
    {
        return;
    }

    int c;
    while (Wire.available())
    {
        c = Wire.read();
    }
    Serial.println(c);
    // * Check to see if the value we got is a valid
    // * game state. if not we ignore it.
    if (c >= IDLE && c <= COMPLETE)
    {
        currentGameState = c;
    }
}

unsigned long lightingInterval = 10;
long previousMillis = 0;

uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;

uint8_t led = 0;
boolean lightsOn = true;

void loop()
{
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis > lightingInterval)
    {

        if (lightsOn && currentGameState == IDLE)
        {
            strip.setPixelColor(led, 255, 0, 255);
        }
        else if (lightsOn && currentGameState == START)
        {
            strip.setPixelColor(led, 0, 255, 0);
        }
        else if (lightsOn && currentGameState == PROCESSING)
        {
            strip.setPixelColor(led, 0, 255, 255);
        }
        else if (lightsOn && currentGameState == COMPLETE)
        {
            strip.setPixelColor(led, 255, 255, 255);
        }
        else
        {
            strip.setPixelColor(led, 0, 0, 0);
        }

        strip.show();

        led++;
        previousMillis = currentMillis;

        if (led > TOTAL_LEDS)
        {
            // lightsOn = !lightsOn;
            led = 0;
        }
    }

    // if (!MainGameSerial.available())
    // {
    //     return;
    // }

    // currentGameState = MainGameSerial.read();

    // // ! hmmm, this whole thing is going to need to be handled via a state machine
    // // ! so we can stop the animations at any point

    // switch (currentGameState)
    // {
    // case IDLE:
    //     colorWipe(strip.Color(255, 0, 255), 10);
    //     break;
    // case START:
    //     colorWipe(strip.Color(75, 0, 130), 10);
    //     break;
    // case PROCESSING:
    //     colorWipe(strip.Color(0, 255, 255), 10);
    //     break;
    // case COMPLETE:
    //     colorWipe(strip.Color(127, 255, 0), 10);
    //     break;
    // }
    // strip.show();
    // return;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, Wheel((i + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++)
    { // 5 cycles of all colors on wheel
        for (i = 0; i < strip.numPixels(); i++)
        {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait)
{
    for (int j = 0; j < 10; j++)
    { //do 10 cycles of chasing
        for (int q = 0; q < 3; q++)
        {
            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, c); //turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, 0); //turn every third pixel off
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait)
{
    for (int j = 0; j < 256; j++)
    { // cycle all 256 colors in the wheel
        for (int q = 0; q < 3; q++)
        {
            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, Wheel((i + j) % 255)); //turn every third pixel on
            }
            strip.show();

            delay(wait);

            for (uint16_t i = 0; i < strip.numPixels(); i = i + 3)
            {
                strip.setPixelColor(i + q, 0); //turn every third pixel off
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85)
    {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
