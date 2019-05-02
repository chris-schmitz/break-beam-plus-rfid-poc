#include <MFRC522.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"
// #include "pitches.h"

#define SS_PIN 13
#define RST_PIN 12

#define break_beam_ONE 11
#define break_beam_TWO 10
#define break_beam_THREE 9
#define break_beam_FOUR 6
#define break_beam_FIVE 5

// #define speaker A4

String bulbasaur = " 04 60 d0 4a e6 4c 81";
String charmander = " 04 78 d2 4a e6 4c 81";
String squirtle = " 04 59 d0 4a e6 4c 81";
String pikachu = " 04 60 d1 4a e6 4c 81";

static const uint8_t PROGMEM
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

int breakBeamState_ONE = 0;
int breakBeamState_TWO = 0;
int breakBeamState_THREE = 0;
int breakBeamState_FOUR = 0;
int breakBeamState_FIVE = 0;

void setup()
{
    matrix.begin(0x70);
    matrix.clear();
    matrix.writeDisplay();

    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println("Starting up");
    SPI.begin();
    mfrc522.PCD_Init();
    mfrc522.PCD_DumpVersionToSerial();

    pinMode(break_beam_ONE, INPUT);
    digitalWrite(break_beam_ONE, HIGH);

    pinMode(break_beam_TWO, INPUT);
    digitalWrite(break_beam_TWO, HIGH);

    pinMode(break_beam_THREE, INPUT);
    digitalWrite(break_beam_THREE, HIGH);

    pinMode(break_beam_FOUR, INPUT);
    digitalWrite(break_beam_FOUR, HIGH);

    pinMode(break_beam_FIVE, INPUT);
    digitalWrite(break_beam_FIVE, HIGH);

    // ready();
    // playMelody(1);
}

int activeIndex;

void loop()
{

    breakBeamState_ONE = digitalRead(break_beam_ONE);
    breakBeamState_TWO = digitalRead(break_beam_TWO);
    breakBeamState_THREE = digitalRead(break_beam_THREE);
    breakBeamState_FOUR = digitalRead(break_beam_FOUR);
    breakBeamState_FIVE = digitalRead(break_beam_FIVE);

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

    if (!breakBeamState_ONE || !breakBeamState_TWO || !breakBeamState_THREE || !breakBeamState_FOUR || !breakBeamState_FIVE)
    {
        Serial.println("===================");
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

    String uid = captureUID();

    Serial.println("captured UID:");
    Serial.println(uid);

    matrix.clear();
    if (uid == charmander)
    {

        matrix.drawBitmap(0, 0, activeSlot[activeIndex], 8, 8, LED_RED);
        // playMelody(1);
    }
    else if (uid == bulbasaur)
    {
        matrix.drawBitmap(0, 0, activeSlot[activeIndex], 8, 8, LED_GREEN);
        // playMelody(2);
    }
    else if (uid == pikachu)
    {
        matrix.drawBitmap(0, 0, activeSlot[activeIndex], 8, 8, LED_YELLOW);
        // playMelody(2);
    }
    matrix.writeDisplay();

    // Dump debug info about the card; PICC_HaltA() is automatically called
    mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
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
