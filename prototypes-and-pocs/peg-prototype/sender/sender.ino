#include <Wire.h>

#define MainGameSerial Serial1
#define followerId 8

enum gameStates
{
    IDLE = 0,
    START,
    PROCESSING,
    COMPLETE
};

uint8_t currentGameState = IDLE;

void setup()
{
    Wire.begin();
    // * Start up our serial ports
    Serial.begin(9600);
    while (!Serial)
        ;
}

void loop()
{
    if (currentGameState == IDLE)
    {
        currentGameState = START;
    }
    else if (currentGameState == START)
    {
        currentGameState = PROCESSING;
    }
    else if (currentGameState == PROCESSING)
    {
        currentGameState = COMPLETE;
    }
    else if (currentGameState == COMPLETE)
    {
        currentGameState = IDLE;
    }
    Serial.print("Sending state: ");
    Serial.println(currentGameState);

    Wire.beginTransmission(followerId);
    Wire.write(currentGameState);
    Wire.endTransmission();

    delay(3000);
}