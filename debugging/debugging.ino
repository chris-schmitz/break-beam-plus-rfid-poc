#include "pitches.h"

// * define all of the pins
// * each of these is marked as an interruptable pin on the M0 Adalogger
#define break_beam_ZERO 15
#define break_beam_ONE 11
#define break_beam_TWO 10
#define break_beam_THREE 9
#define break_beam_FOUR 6
#define break_beam_FIVE 5

#define speaker A2

// * Init some state tracking vars
volatile int breakBeamState_ZERO = 1;
volatile int breakBeamState_ONE = 1;
volatile int breakBeamState_TWO = 1;
volatile int breakBeamState_THREE = 1;
volatile int breakBeamState_FOUR = 1;
volatile int breakBeamState_FIVE = 1;

int melody[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
    4, 8, 8, 4, 4, 4, 4, 4};

void ready()
{
    Serial.println("playing tones");
    for (int thisNote = 0; thisNote < 8; thisNote++)
    {

        // to calculate the note duration, take one second divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(speaker, melody[thisNote], noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(speaker);
    }
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ;
    Serial.println("Starting up");

    // * Set up each of the break-beam pins
    // * Note that this is a modification of the original pinout settings that I was using
    // * when I was trying to query the state in the loop.
    // * The original settings looked like this:

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

    Serial.println("Ready to debug");
    ready();
}

void loop()
{
    // * Check our state. In theory these should be updated by the ISR, but I'm not seeing the changes
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
    // delay(100);
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