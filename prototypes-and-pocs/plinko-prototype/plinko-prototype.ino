/**
 * | Holy cow this needs to be cleaned up and abstracted, if not in this prototype DEFINITELY
 * | in the final version!
 **/

#include <MFRC522.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include <Wire.h>

#include "RFIDs.h"
#include "Bitmaps.h"

#define SS_PIN 13
#define RST_PIN 12

#define break_beam_ZERO 15
#define break_beam_ONE 11
#define break_beam_TWO 10
#define break_beam_THREE 9
#define break_beam_FOUR 18
#define break_beam_FIVE 5

#define speaker A2

#define PEG_CONTROLLER_ID 8

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

const uint8_t *activeSlot[] PROGMEM = {slot_1_bmp, slot_2_bmp, slot_3_bmp, slot_4_bmp, slot_5_bmp};

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

String content;

volatile bool spinnerOn = false;

volatile int breakBeamState_ZERO = 1;
volatile int breakBeamState_ONE = 1;
volatile int breakBeamState_TWO = 1;
volatile int breakBeamState_THREE = 1;
volatile int breakBeamState_FOUR = 1;
volatile int breakBeamState_FIVE = 1;

void setup()
{
    Serial.begin(9600);
    // Serial.begin(115200);
    Serial1.begin(9600); // * for messaging to second controller
    while (!Serial)
        ;
    Serial.println("Serial ports open");

    resetBreakBeamState();

    Serial.println("Starting up");
    SPI.begin();
    mfrc522.PCD_Init();
    mfrc522.PCD_DumpVersionToSerial();

    pinMode(break_beam_ZERO, INPUT);
    digitalWrite(break_beam_ZERO, HIGH);
    attachInterrupt(break_beam_ZERO, setBreakbeamState, FALLING);

    pinMode(break_beam_ONE, INPUT);
    digitalWrite(break_beam_ONE, HIGH);
    attachInterrupt(break_beam_ONE, setBreakbeamState, FALLING);

    pinMode(break_beam_TWO, INPUT);
    digitalWrite(break_beam_TWO, HIGH);
    attachInterrupt(break_beam_TWO, setBreakbeamState, FALLING);

    pinMode(break_beam_THREE, INPUT);
    digitalWrite(break_beam_THREE, HIGH);
    attachInterrupt(break_beam_THREE, setBreakbeamState, FALLING);

    pinMode(break_beam_FOUR, INPUT);
    digitalWrite(break_beam_FOUR, HIGH);
    attachInterrupt(break_beam_FOUR, setBreakbeamState, FALLING);

    pinMode(break_beam_FIVE, INPUT);
    digitalWrite(break_beam_FIVE, HIGH);
    attachInterrupt(break_beam_FIVE, setBreakbeamState, FALLING);

    Wire.begin();
    matrix.begin(0x70);
    matrix.setRotation(3);

    Serial.println("Setup complete");
    ready();

    Serial.println("Exiting setup function");
}

void ready()
{
    runSpinner(LED_GREEN);
    messagePegs(IDLE);
}

void messagePegs(gameStates state)
{
    Serial.print("Messaging pegs with enum:");
    Serial.println(state);
    Serial1.write(state);
}

void runSpinner(int color)
{
    Serial.println("Running spinner");
    spinnerOn = true;
    matrix.clear();
    matrix.writeDisplay();

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_vertical, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_forwardSlash, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_horizontal, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_backSlash, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_vertical, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_forwardSlash, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_horizontal, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_backSlash, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_vertical, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_forwardSlash, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_horizontal, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.drawBitmap(0, 0, spinner_backSlash, 8, 8, color);
    matrix.writeDisplay();
    delay(100);

    matrix.clear();
    matrix.writeDisplay();
    spinnerOn = false;
}

int activeIndex;

void loop()
{

    if (!breakBeamState_ZERO || !breakBeamState_ONE || !breakBeamState_TWO || !breakBeamState_THREE || !breakBeamState_FOUR || !breakBeamState_FIVE)
    {
        Serial.println("");
        Serial.println("===================");
        Serial.print("breakBeamState_ZERO: ");
        Serial.println(breakBeamState_ZERO);
        Serial.print("breakBeamState_ONE: ");
        Serial.println(breakBeamState_ONE);
        Serial.print("breakBeamState_TWO: ");
        Serial.println(breakBeamState_TWO);
        Serial.print("breakBeamState_THREE: ");
        Serial.println(breakBeamState_THREE);
        Serial.print("breakBeamState_FOUR: ");
        Serial.println(breakBeamState_FOUR);
        Serial.print("breakBeamState_FIVE: ");
        Serial.println(breakBeamState_FIVE);
        Serial.println("===================");
        Serial.println("");
    }

    if (!breakBeamState_ZERO)
    {
        breakBeamState_ZERO = true;
        Serial.println("Break Beam 0");
        resetGameplay(); //! I don't think we need this now that we're resetting in the conditionals
        messagePegs(START);
        // playReady();
    }

    if (!breakBeamState_ONE)
    {
        breakBeamState_ONE = true;
        Serial.println("Break beam 1");
        activeIndex = 0;
        messagePegs(GATE_1);
    }
    else if (!breakBeamState_TWO)
    {
        breakBeamState_TWO = true;
        Serial.println("Break beam 2");
        activeIndex = 1;
        messagePegs(GATE_2);
    }
    else if (!breakBeamState_THREE)
    {
        breakBeamState_THREE = true;
        Serial.println("Break beam 3");
        activeIndex = 2;
        messagePegs(GATE_3);
    }
    else if (!breakBeamState_FOUR)
    {
        breakBeamState_FOUR = true;
        Serial.println("Break beam 4");
        activeIndex = 3;
        messagePegs(GATE_4);
    }
    else if (!breakBeamState_FIVE)
    {
        breakBeamState_FIVE = true;
        Serial.println("Break beam 5");
        activeIndex = 4;
        messagePegs(GATE_5);
    }
    else
    {
        // playIdle();
    }

    if (!breakBeamState_ZERO || !breakBeamState_ONE || !breakBeamState_TWO || !breakBeamState_THREE || !breakBeamState_FOUR || !breakBeamState_FIVE)
    {
        Serial.println("===================");
        Serial.print("breakBeamState_ZERO: ");
        Serial.println(breakBeamState_ZERO);
        Serial.print("breakBeamState_ONE: ");
        Serial.println(breakBeamState_ONE);
        Serial.print("breakBeamState_TWO: ");
        Serial.println(breakBeamState_TWO);
        Serial.print("breakBeamState_THREE: ");
        Serial.println(breakBeamState_THREE);
        Serial.print("breakBeamState_FOUR: ");
        Serial.println(breakBeamState_FOUR);
        Serial.print("breakBeamState_FIVE: ");
        Serial.println(breakBeamState_FIVE);
        Serial.println("===================");
    }
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        // Serial.println("can't see tag");
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
    {
        // Serial.println("can't read tag");
        return;
    }
    Serial.println("see a tag");

    messagePegs(PROCESSING);

    String uid = captureUID();

    Serial.println("captured UID:");
    Serial.println(uid);

    matrix.clear();
    if (uid == cloud)
    {
        Serial.println("Drawing red!");
        matrix.drawBitmap(0, 0, activeSlot[activeIndex], 8, 8, LED_RED);
        runSpinner(LED_RED);
        runSpinner(LED_RED);
        runSpinner(LED_RED);
    }
    else if (uid == cupcake)
    {
        Serial.println("Drawing green!");
        matrix.drawBitmap(0, 0, activeSlot[activeIndex], 8, 8, LED_GREEN);
        runSpinner(LED_GREEN);
        runSpinner(LED_GREEN);
        runSpinner(LED_GREEN);
    }
    else if (uid == crown)
    {
        Serial.println("Drawing yellow!");
        matrix.drawBitmap(0, 0, activeSlot[activeIndex], 8, 8, LED_YELLOW);
        runSpinner(LED_YELLOW);
        runSpinner(LED_YELLOW);
        runSpinner(LED_YELLOW);
    }
    matrix.writeDisplay();

    messagePegs(COMPLETE);
    // * giving time for a complete status before switching back to idle
    delay(3000);

    messagePegs(IDLE);

    // * leaving this in during dev for easy troubleshooting
    // Dump debug info about the card; PICC_HaltA() is automatically called
    // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

String captureUID()
{
    content = "";
    for (byte i = 0; i < mfrc522.uid.size; i++)
    {
        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    // * leaving in for trouble shooting purposes
    // Serial.println(content);
    return content;
}

void resetGameplay()
{
    Serial.println("resetting gameplay");
    matrix.clear();
    matrix.writeDisplay();
}

// ! this may not be needed anymore
void resetBreakBeamState()
{
    Serial.println("resetting break beam statestate");
    breakBeamState_ZERO = 1;
    breakBeamState_ONE = 1;
    breakBeamState_TWO = 1;
    breakBeamState_THREE = 1;
    breakBeamState_FOUR = 1;
    breakBeamState_FIVE = 1;
}

void setBreakbeamState()
{
    // * Reading in the state of the break beam pins
    breakBeamState_ZERO = digitalRead(break_beam_ZERO);
    breakBeamState_ONE = digitalRead(break_beam_ONE);
    breakBeamState_TWO = digitalRead(break_beam_TWO);
    breakBeamState_THREE = digitalRead(break_beam_THREE);
    breakBeamState_FOUR = digitalRead(break_beam_FOUR);
    breakBeamState_FIVE = digitalRead(break_beam_FIVE);
}