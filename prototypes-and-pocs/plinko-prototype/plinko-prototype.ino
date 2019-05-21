#include <MFRC522.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
#include "pitches.h"
#include <Wire.h>

#define SS_PIN 13
#define RST_PIN 12

#define break_beam_ZERO 15
#define break_beam_ONE 11
#define break_beam_TWO 10
#define break_beam_THREE 9
#define break_beam_FOUR 6
#define break_beam_FIVE 5

#define speaker A2

#define PEG_CONTROLLER_ID 8

enum gameStates
{
    IDLE = 0,
    START,
    PROCESSING,
    COMPLETE
};

String cupcake = " 04 aa cb 4a e6 4c 80";
String diamond = " 04 a5 e1 4a e6 4c 80";
String crown = " 04 d1 cd 4a e6 4c 80";

String bulbasaur = " 04 60 d0 4a e6 4c 81";
String charmander = " 04 78 d2 4a e6 4c 81";
String squirtle = " 04 59 d0 4a e6 4c 81";
String pikachu = " 04 60 d1 4a e6 4c 81";

int idleMelody[] = {
    NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3};

int readyMelody[] = {
    NOTE_C3, NOTE_G3};

int completionMelody[] = {
    NOTE_G3, NOTE_C3, NOTE_G3};

int idleDurations[] = {
    4, 4, 4, 4, 1};
int readyDurations[] = {
    4, 4};
int completionDurations[] = {
    4, 4, 2};

static const uint8_t PROGMEM
    all_bmp[] =
        {
            B11111111,
            B11111111,
            B11111111,
            B11111111,
            B11111111,
            B11111111,
            B11111111,
            B11111111},
    slot_1_bmp[] =
        {
            B00000000,
            B00011000,
            B00011000,
            B00111000,
            B00011000,
            B00011000,
            B00011000,
            B01111110},
    slot_2_bmp[] =
        {
            B00000000,
            B00111100,
            B01100110,
            B00000110,
            B00001100,
            B00110000,
            B01100000,
            B01111110},
    slot_3_bmp[] =
        {
            B00000000,
            B00111100,
            B01100110,
            B00000110,
            B00011100,
            B00000110,
            B01100110,
            B00111100},
    slot_4_bmp[] =
        {
            B00000000,
            B00001100,
            B00011100,
            B00101100,
            B01001100,
            B01111110,
            B00001100,
            B00001100},
    slot_5_bmp[] =
        {
            B00000000,
            B01111110,
            B01100000,
            B01111100,
            B00000110,
            B00000110,
            B01100110,
            B00111100};

const uint8_t *activeSlot[] PROGMEM = {slot_1_bmp, slot_2_bmp, slot_3_bmp, slot_4_bmp, slot_5_bmp};

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

String content;

volatile int breakBeamState_ZERO = 1;
volatile int breakBeamState_ONE = 1;
volatile int breakBeamState_TWO = 1;
volatile int breakBeamState_THREE = 1;
volatile int breakBeamState_FOUR = 1;
volatile int breakBeamState_FIVE = 1;

void setup()
{
    Wire.begin();

    messagePegs(IDLE);

    resetBreakBeamState();

    matrix.begin(0x70);
    matrix.setRotation(3);

    matrix.clear();
    matrix.drawBitmap(0, 0, all_bmp, 8, 8, LED_GREEN);
    matrix.writeDisplay();
    delay(500);

    Serial.begin(115200);
    // while (!Serial)
    //     ;
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

    Serial.println("Setup complete");
    // playReady();
    Serial.println("Exiting setup function");
}

void playReady()
{
    Serial.println("Playing ready tone");
    for (int thisNote = 0; thisNote < 8; thisNote++)
    {

        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / readyDurations[thisNote];
        tone(speaker, readyMelody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(speaker);
    }
}
void playIdle()
{
    Serial.println("Playing ready tone");
    for (int thisNote = 0; thisNote < 8; thisNote++)
    {

        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / idleDurations[thisNote];
        tone(speaker, idleMelody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(speaker);
    }
}
void playCompletion()
{
    Serial.println("Playing ready tone");
    for (int thisNote = 0; thisNote < 8; thisNote++)
    {

        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / completionDurations[thisNote];
        tone(speaker, completionMelody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(speaker);
    }
}

void messagePegs(gameStates state)
{
    Serial.print("sending game state");
    Wire.beginTransmission(PEG_CONTROLLER_ID);
    Wire.write(state);
    Wire.endTransmission();
}

int activeIndex;

void loop()
{
    // Serial.println("at top of loop");
    // delay(100);
    // return;
    if (!breakBeamState_ZERO)
    {
        Serial.println("Resetting state");
        resetState();
        messagePegs(START);
        // playReady();
    }

    if (!breakBeamState_ONE)
    {
        activeIndex = 0;
    }
    else if (!breakBeamState_TWO)
    {
        activeIndex = 1;
    }
    else if (!breakBeamState_THREE)
    {
        activeIndex = 2;
    }
    else if (!breakBeamState_FOUR)
    {
        activeIndex = 3;
    }
    else if (!breakBeamState_FIVE)
    {
        activeIndex = 4;
    }
    else
    {
        // playIdle();
    }

    if (!breakBeamState_ONE || !breakBeamState_TWO || !breakBeamState_THREE || !breakBeamState_FOUR || !breakBeamState_FIVE)
    {
        // playCompletion();
    }
    if (!breakBeamState_ZERO || !breakBeamState_ONE || !breakBeamState_TWO || !breakBeamState_THREE || !breakBeamState_FOUR || !breakBeamState_FIVE)
    {
        Serial.println("===================");
        Serial.println(breakBeamState_ZERO);
        Serial.println(breakBeamState_ONE);
        Serial.println(breakBeamState_TWO);
        Serial.println(breakBeamState_THREE);
        Serial.println(breakBeamState_FOUR);
        Serial.println(breakBeamState_FIVE);
        Serial.println("===================");
    }

    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent())
    {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
    {
        return;
    }

    messagePegs(PROCESSING);

    String uid = captureUID();

    Serial.println("captured UID:");
    Serial.println(uid);

    matrix.clear();
    if (uid == diamond)
    // if (uid == charmander)
    {
        Serial.println("Drawing red!");
        matrix.drawBitmap(0, 0, activeSlot[activeIndex], 8, 8, LED_RED);
    }
    else if (uid == cupcake)
    // else if (uid == bulbasaur)
    {
        Serial.println("Drawing green!");
        matrix.drawBitmap(0, 0, activeSlot[activeIndex], 8, 8, LED_GREEN);
    }
    else if (uid == crown)
    // else if (uid == pikachu)
    {
        Serial.println("Drawing yellow!");
        matrix.drawBitmap(0, 0, activeSlot[activeIndex], 8, 8, LED_YELLOW);
    }
    matrix.writeDisplay();

    // ! adding this artificial delay to represent the time between Reading
    // ! the RFID tag and the receipt printer finishing its print.
    delay(3000);
    messagePegs(COMPLETE);

    // ! adding this artificial delay to represent the time between Reading
    // ! the RFID tag and the receipt printer finishing its print.
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
        // Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
        // Serial.print(mfrc522.uid.uidByte[i], HEX);
        // Serial.println();

        content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
        content.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    // Serial.println("Content:");
    // Serial.println(content);
    return content;
}

void resetState()
{
    Serial.println("resetting state");
    matrix.clear();
    matrix.writeDisplay();

    resetBreakBeamState();
}

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