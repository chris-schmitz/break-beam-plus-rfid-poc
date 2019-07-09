#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "src/tunePlayer/TunePlayer.h"

#define PEGS_PIN 2
#define GATES_PIN 1
#define SPEAKER 0

#define PEGS_TOTAL_LEDS 22
#define GATES_TOTAL_LEDS 15

Adafruit_NeoPixel pegs = Adafruit_NeoPixel(PEGS_TOTAL_LEDS, PEGS_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel gates = Adafruit_NeoPixel(GATES_TOTAL_LEDS, GATES_PIN, NEO_GRB + NEO_KHZ800);

TunePlayer tunes = TunePlayer(SPEAKER);

// TODO: UGH THIS IS DRIVING ME NUTS!!!!
// | RENAME MELODY TO TUNE EVERWHERE WHERE WE'RE NOT TALKING SPECIFICALLY ABOUT A MELODY!!
TUNES tuneToPlay;

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

// | 1 through 4 = gates
// | 0 = reset
uint8_t activeGate;
uint8_t previouslyActiveGate;
uint8_t *activeGateLedArray;

// boolean gateState[] = {false, false, false, false, false};
uint8_t gate5Leds[] = {0, 1, 2};
uint8_t gate4Leds[] = {3, 4, 5};
uint8_t gate3Leds[] = {6, 7, 8};
uint8_t gate2Leds[] = {9, 10, 11};
uint8_t gate1Leds[] = {12, 13, 14};

uint32_t COLOR_OFF = gates.Color(0, 0, 0);
uint32_t COLOR_GREEN = gates.Color(0, 255, 0);
uint32_t COLOR_PURPLE = gates.Color(128, 0, 255);

void setup()
{
    // * Start up our serial ports
    Serial.begin(9600);
    Serial1.begin(9600);
    // * Leaving this in here b/c it's nice to be able to pause for troubleshooting purposes
    // while (!Serial)
    //     ;

    // * Start up our ledpegs
    pegs.begin();
    pegs.show();
    gates.begin();
    gates.show();
    ready();
}

void ready()
{
    Serial.println("ready");

    colorWipe(pegs.Color(255, 0, 0), 25, pegs);
    colorWipe(pegs.Color(0, 255, 0), 25, pegs);
    colorWipe(pegs.Color(0, 0, 255), 25, pegs);

    colorWipe(pegs.Color(255, 0, 0), 25, gates);
    colorWipe(pegs.Color(0, 255, 0), 25, gates);
    colorWipe(pegs.Color(0, 0, 255), 25, gates);

    colorWipe(COLOR_OFF, 5, pegs);
    colorWipe(COLOR_OFF, 5, gates);

    tunes.playMelody(TUNE_READY);
}
void colorWipe(uint32_t c, uint8_t wait, Adafruit_NeoPixel &strip)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void checkForMessagesFromLeader()
{
    if (!Serial1.available())
    {
        return;
    }

    Serial.println("Got message from leader");
    Serial.print("bytes: ");

    int c;
    c = Serial1.read();
    Serial.println(c);
    // * Check to see if the value we got is a valid
    // * game state. if not we ignore it.
    if (c >= IDLE && c <= GATE_5)
    {
        currentGameState = c;
    }
    else
    {
        Serial.println("Illegal game state");
        return; // ? do we want an early return??
    }

    Serial.println(currentGameState);

    switch (currentGameState)
    {
    case GATE_1:
        activeGate = 1;
        break;
    case GATE_2:
        activeGate = 2;
        break;
    case GATE_3:
        activeGate = 3;
        break;
    case GATE_4:
        activeGate = 4;
        break;
    case GATE_5:
        activeGate = 5;
        break;
    case IDLE:
        tuneToPlay = TUNE_IDLE;
        activeGate = 0;
        break;
    case START:
        tuneToPlay = TUNE_READY;
        activeGate = 0;
        break;
    case PROCESSING:
        tuneToPlay = TUNE_PROCESSING;
        break;
    case COMPLETE:
        tuneToPlay = TUNE_COMPLETE;
        break;
    }
}

unsigned long lightingInterval = 10;
long previousMillis = 0;

unsigned long checkForIdleInterval = 10000;
long checkForIdlePreviousMillis = 0;

uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;

uint8_t led = 0;
boolean lightsOn = true;

void loop()
{
    unsigned long currentMillis = millis();

    checkForMessagesFromLeader();

    if (tunes.currentMelody == tuneToPlay)
    {
        tunes.playMelodyWithoutDelay();
    }
    else
    {
        tunes.stopPlaying();
        tunes.reset();
        tunes.setActiveMelody(tuneToPlay);
        tunes.playMelodyWithoutDelay();
    }

    // * if we're still idling, play the idle tune after a bit
    if (currentMillis - checkForIdlePreviousMillis > checkForIdleInterval)
    {
        if (!tunes.playingMelody && tunes.currentMelody == TUNE_IDLE)
        {
            tunes.reset();
            tunes.setActiveMelody(TUNE_IDLE);
            tunes.playMelodyWithoutDelay();
        }
    }

    if (currentGameState == COMPLETE)
    {
        activeGate = 0;
    }

    if (previouslyActiveGate != activeGate)
    {
        handleGates();
    }

    if (currentMillis - previousMillis > lightingInterval)
    {
        handlePegs();
        previousMillis = currentMillis;
    }
}

uint8_t idleColor = 0;
boolean successToggle = true;
boolean pegsFlooded = false;

void handlePegs()
{
    // * now that we're getting a bit more fancy as far as how we handle peg lighting
    // * state it may be worth abstracting some of this logic or possibly classing the
    // * peg handler up.
    if (lightsOn && currentGameState == IDLE)
    {
        lightingInterval = 10;
        pegsFlooded = false;
        pegs.setPixelColor(led, Wheel((led + idleColor) & 255));
        idleColor++;
    }
    else if (lightsOn && currentGameState == START)
    {
        if (!pegsFlooded)
        {
            floodPegs(COLOR_GREEN);
        }
        lightingInterval = 80;
        pegs.setPixelColor(PEGS_TOTAL_LEDS - led - 3, COLOR_PURPLE);
        pegs.setPixelColor(PEGS_TOTAL_LEDS - led - 2, COLOR_PURPLE);
        pegs.setPixelColor(PEGS_TOTAL_LEDS - led - 1, COLOR_PURPLE);
        pegs.setPixelColor(PEGS_TOTAL_LEDS - led, COLOR_GREEN);
    }
    else if (lightsOn && currentGameState == PROCESSING)
    {
        lightingInterval = 10;
        pegsFlooded = false;
        pegs.setPixelColor(led, 0, 255, 255);
    }
    else if (lightsOn && currentGameState == COMPLETE)
    {
        lightingInterval = 5;
        pegsFlooded = false;
        if (successToggle)
        {
            pegs.setPixelColor(led, 255, 255, 255);
        }
        else
        {
            pegs.setPixelColor(led, 255, 255, 0);
        }
        successToggle = !successToggle;
    }
    else
    {
        //        pegs.setPixelColor(led, 0, 0, 0);
    }

    pegs.show();

    led++;

    if (led > PEGS_TOTAL_LEDS)
    {
        //        lightsOn = !lightsOn;
        led = 0;
    }
}

void floodPegs(uint32_t color)
{
    for (uint8_t i = 0; i < PEGS_TOTAL_LEDS; i++)
    {
        pegs.setPixelColor(i, color);
    }
    pegs.show();
    pegsFlooded = true;
}

void handleGates()
{
    // * We only need one gate on at a time so start with all off
    for (uint8_t i = 0; i < GATES_TOTAL_LEDS; i++)
    {
        gates.setPixelColor(i, COLOR_OFF);
    }

    switch (activeGate)
    {
    // * Note that we're intentionally not setting a state for activeGate == 0
    // * because if we get active gate 0 we want to leave all gates off.
    case 1:
        activeGateLedArray = gate1Leds;
        break;
    case 2:
        activeGateLedArray = gate2Leds;
        break;
    case 3:
        activeGateLedArray = gate3Leds;
        break;
    case 4:
        activeGateLedArray = gate4Leds;
        break;
    case 5:
        activeGateLedArray = gate5Leds;
        break;
    }

    if (currentGameState != COMPLETE)
    {
        Serial.println("Not complete");
        for (uint8_t i = 0; i < 3; i++)
        {
            gates.setPixelColor(activeGateLedArray[i], gates.Color(0, 255, 255));
        }
    }
    gates.show();
    previouslyActiveGate = activeGate;
}

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
