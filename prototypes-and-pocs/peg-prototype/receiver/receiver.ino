#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "pitches.h"

#define PEGS_PIN 4
#define GATES_PIN 1
#define speaker 2

// #define TOTAL_LEDS 8
#define PEGS_TOTAL_LEDS 22
#define GATES_TOTAL_LEDS 15

Adafruit_NeoPixel pegs = Adafruit_NeoPixel(PEGS_TOTAL_LEDS, PEGS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel gates = Adafruit_NeoPixel(GATES_TOTAL_LEDS, GATES_PIN, NEO_GRB + NEO_KHZ800);

// * this enum should prob be renamed to something more generic like `messagesFromLeader` or something
enum gameStates
{
    IDLE = 0,
    START,
    PROCESSING,
    COMPLETE,
    GATE_1,
    GATE_2,
    GATE_3,
    GATE_4,
    GATE_5
};

int currentGameState = IDLE;

// * states for controlling the gate led strips
boolean gateState[] = {false, false, false, false, false};
uint8_t gate5Leds[] = {0, 1, 2};
uint8_t gate4Leds[] = {3, 4, 5};
uint8_t gate3Leds[] = {6, 7, 8};
uint8_t gate2Leds[] = {9, 10, 11};
uint8_t gate1Leds[] = {12, 13, 14};

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

    // * Start up our ledpegs
    pegs.begin();
    pegs.show();
    gates.begin();
    gates.show();
    ready();
}

void ready()
{
    colorWipe(pegs.Color(255, 0, 0), 25);
    colorWipe(pegs.Color(0, 255, 0), 25);
    colorWipe(pegs.Color(0, 0, 255), 25);
    colorWipe(pegs.Color(0, 0, 0), 25);
    Serial.println("ready");
}
void colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < pegs.numPixels(); i++)
    {
        pegs.setPixelColor(i, c);
        pegs.show();
        delay(wait);
    }
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

    // * this is ultra crude code. After proving the logic out come back and optimize
    if (c == GATE_1)
    {
        gateState[0] = true;
    }
    if (c == GATE_2)
    {
        gateState[1] = true;
    }
    if (c == GATE_3)
    {
        gateState[2] = true;
    }
    if (c == GATE_4)
    {
        gateState[3] = true;
    }
    if (c == GATE_5)
    {
        gateState[4] = true;
    }
    if (c == IDLE || c == START)
    {
        for (uint8_t i = 0; i < 5; i++)
        {
            gateState[i] = false;
        }
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

    handleGates();

    if (currentMillis - previousMillis > lightingInterval)
    {

        if (lightsOn && currentGameState == IDLE)
        {
            pegs.setPixelColor(led, 255, 0, 255);
            playIdle();
        }
        else if (lightsOn && currentGameState == START)
        {
            pegs.setPixelColor(led, 0, 255, 0);
        }
        else if (lightsOn && currentGameState == PROCESSING)
        {
            pegs.setPixelColor(led, 0, 255, 255);
        }
        else if (lightsOn && currentGameState == COMPLETE)
        {
            pegs.setPixelColor(led, 255, 255, 255);
        }
        else
        {
            pegs.setPixelColor(led, 0, 0, 0);
        }

        pegs.show();

        led++;
        previousMillis = currentMillis;

        if (led > PEGS_TOTAL_LEDS)
        {
            // lightsOn = !lightsOn;
            led = 0;
        }
    }
}

// ! this is a really crude and repatitve way of handling the gate activation. After
// ! proving it out come back and put the dynamic gate led array selection structure in place
void handleGates()
{
    if (gateState[0] == true)
    {
        currentGameState = IDLE;
        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate1Leds[i], gates.Color(0, 255, 255));
        }
    }
    else
    {
        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate1Leds[i], gates.Color(0, 0, 0));
        }
    }
    if (gateState[1] == true)
    {
        currentGameState = IDLE;
        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate2Leds[i], gates.Color(0, 255, 255));
        }
    }
    else
    {

        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate2Leds[i], gates.Color(0, 0, 0));
        }
    }
    if (gateState[2] == true)
    {
        currentGameState = IDLE;
        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate3Leds[i], gates.Color(0, 255, 255));
        }
    }
    else
    {

        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate3Leds[i], gates.Color(0, 0, 0));
        }
    }
    if (gateState[3] == true)
    {
        currentGameState = IDLE;
        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate4Leds[i], gates.Color(0, 255, 255));
        }
    }
    else
    {

        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate4Leds[i], gates.Color(0, 0, 0));
        }
    }
    if (gateState[4] == true)
    {
        currentGameState = IDLE;
        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate5Leds[i], gates.Color(0, 255, 255));
        }
    }
    else
    {

        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(gate5Leds[i], gates.Color(0, 0, 0));
        }
    }
    gates.show();
}

// | === sound effects ===
int idleMelody[] = {
    NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3};
int idleDurations[] = {
    4, 4, 4, 4, 2};

// uint8_t totalNotes = 5; // ! later come back and replace this with a calc
// * To get around the delay I'm going to try to replace the loop
// * with an incrementing position variable
// uint8_t idlePosition = 0;
unsigned long tonePreviousMillis = 0;
const long pauseBetweenNotes = 1;
boolean outputTone = false;

void playIdle()
{
    unsigned long currentMillis = millis();

    if (outputTone)
    {
        // * Currently outputting a tone
        // * Check to see if it's been long enough to turn off the tone
        if (currentMillis - tonePreviousMillis >= idleDurations[0])
        {
            tonePreviousMillis = currentMillis;
            noTone(speaker);
            outputTone = false;
        }
        else
        {
            // * we're currntly in a pause
            // * check to see if it's been long enough to turn on the next tone
            if (currentMillis - tonePreviousMillis >= pauseBetweenNotes)
            {
                tonePreviousMillis = currentMillis;
                tone(speaker, idleMelody[0]);
                outputTone = true;
            }
        }
    }
}

void rainbow(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        for (i = 0; i < pegs.numPixels(); i++)
        {
            pegs.setPixelColor(i, Wheel((i + j) & 255));
        }
        pegs.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++)
    { // 5 cycles of all colors on wheel
        for (i = 0; i < pegs.numPixels(); i++)
        {
            pegs.setPixelColor(i, Wheel(((i * 256 / pegs.numPixels()) + j) & 255));
        }
        pegs.show();
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
            for (uint16_t i = 0; i < pegs.numPixels(); i = i + 3)
            {
                pegs.setPixelColor(i + q, c); //turn every third pixel on
            }
            pegs.show();

            delay(wait);

            for (uint16_t i = 0; i < pegs.numPixels(); i = i + 3)
            {
                pegs.setPixelColor(i + q, 0); //turn every third pixel off
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
            for (uint16_t i = 0; i < pegs.numPixels(); i = i + 3)
            {
                pegs.setPixelColor(i + q, Wheel((i + j) % 255)); //turn every third pixel on
            }
            pegs.show();

            delay(wait);

            for (uint16_t i = 0; i < pegs.numPixels(); i = i + 3)
            {
                pegs.setPixelColor(i + q, 0); //turn every third pixel off
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
        return pegs.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if (WheelPos < 170)
    {
        WheelPos -= 85;
        return pegs.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    WheelPos -= 170;
    return pegs.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
