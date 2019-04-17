#include <MFRC522.h>
#include <SPI.h>
#include "pitches.h"

#define SS_PIN 19
#define RST_PIN 5

#define break_beam_left 10
#define break_beam_right 11

#define break_beam_top 12

#define led_left 16
#define led_right 17

#define speaker A4

String bulbasaur = " 04 60 d0 4a e6 4c 81";
String charmander = " 04 78 d2 4a e6 4c 81";
String squirtle = " 04 59 d0 4a e6 4c 81";
String pikachu = " 04 60 d1 4a e6 4c 81";

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

String content;

int beamStateLeft = 0;
int beamStateRight = 0;
int beamStateTop = 0;

const int melody1_size = 5;
int melody1[melody1_size] = {
    NOTE_C3, NOTE_D3, NOTE_E3, NOTE_F3, NOTE_G3};
int melody1_durations[melody1_size] = {
    8, 8, 8, 8, 2};

const int melody2_size = 5;
int melody2[melody2_size] = {NOTE_G3, NOTE_F3, NOTE_E3, NOTE_D3, NOTE_C3};
int melody2_durations[melody2_size] = {
    8, 8, 8, 8, 2};

void playMelody(int melodyNumber);

void ready()
{
    // * ready
    digitalWrite(led_left, HIGH);
    digitalWrite(led_right, HIGH);
    delay(100);
    digitalWrite(led_left, LOW);
    digitalWrite(led_right, LOW);
    delay(100);
    digitalWrite(led_left, HIGH);
    digitalWrite(led_right, HIGH);
    delay(100);
    digitalWrite(led_left, LOW);
    digitalWrite(led_right, LOW);
    delay(100);
}

void setup()
{
    Serial.begin(9600);
    // while (!Serial)
    //     ;
    Serial.println("Starting up");
    SPI.begin();
    mfrc522.PCD_Init();
    mfrc522.PCD_DumpVersionToSerial();

    pinMode(break_beam_left, INPUT);
    digitalWrite(break_beam_left, HIGH);
    pinMode(break_beam_right, INPUT);
    digitalWrite(break_beam_right, HIGH);
    pinMode(break_beam_top, INPUT);
    digitalWrite(break_beam_top, HIGH);

    pinMode(led_left, OUTPUT);
    pinMode(led_right, OUTPUT);

    ready();
    playMelody(1);
}

void loop()
{

    beamStateLeft = digitalRead(break_beam_left);
    beamStateRight = digitalRead(break_beam_right);
    beamStateTop = digitalRead(break_beam_top);

    // Serial.println("===================");
    // Serial.println(beamStateLeft);
    // Serial.println(beamStateRight);
    // Serial.println(beamStateTop);
    // Serial.println("===================");

    // * if the top beam is broken it means we're restarting te         experience
    if (!beamStateTop)
    {
        reset();
        Serial.println("Top is broken");
    }

    if (!beamStateLeft)
    {
        Serial.println("Beam 1 broken");
        digitalWrite(led_left, HIGH);
    }

    if (!beamStateRight)
    {
        Serial.println("Beam 2 broken");
        digitalWrite(led_right, HIGH);
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

    // Serial.println("captured UID:");
    // Serial.println(uid);

    if (uid == charmander)
    {
        playMelody(1);
    }
    else if (uid == bulbasaur)
    {

        playMelody(2);
    }

    // // Dump debug info about the card; PICC_HaltA() is automatically called
    // mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}

void reset()
{
    digitalWrite(led_left, LOW);
    digitalWrite(led_right, LOW);
}

void playMelody(int melodyNumber)
{
    Serial.println("Playing tone");
    int *notes;
    int *durations;
    int arraySize;
    if (melodyNumber == 1)
    {
        notes = melody1;
        durations = melody1_durations;
        arraySize = melody1_size;
    }
    else if (melodyNumber == 2)
    {
        notes = melody2;
        durations = melody2_durations;
        arraySize = melody2_size;
    }

    for (int i = 0; i < arraySize; i++)
    {
        // 1000 == 1 second
        int noteDuration = 1000 / durations[i];
        tone(speaker, notes[i], noteDuration);

        int pauseBetweenNotes = noteDuration * 1.3;
        delay(pauseBetweenNotes);
        noTone(speaker);
    }
}

/**
 *
 */

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