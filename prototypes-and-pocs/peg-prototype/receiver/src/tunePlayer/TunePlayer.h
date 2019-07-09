#ifndef TUNE_PLAYER
#define TUNE_PLAYER
#include "pitches.h"
#include "Arduino.h"

enum TUNES
{
    TUNE_SHAVE_AND_A_HAIRCUT = 0,
    TUNE_IDLE,
    TUNE_READY,
    TUNE_PROCESSING,
    TUNE_COMPLETE,
};

class TunePlayer
{
    uint8_t speakerPin;

    // * Durations and frequencies for the different playable melodies
    // | Note that the frequencies and durations are set in the implmentation file.
    // | At the moment there's no way of passing in the info via public method.
    static int length_shaveAndAHairCut;
    static int melody_shaveAndAHairCut[];
    static int durations_shaveAndAHairCut[];

    static int length_READY;
    static int melody_READY[];
    static int durations_READY[];

    static int length_PROCESSING;
    static int melody_PROCESSING[];
    static int durations_PROCESSING[];

    static int length_COMPLETE;
    static int melody_COMPLETE[];
    static int durations_COMPLETE[];

    static int length_IDLE;
    static int melody_IDLE[];
    static int durations_IDLE[];

    // * internal pointers used in the dynamic melody selection
    int activeLength;
    int *activeMelody;
    int *activeDurations;
    // | hmmm, consider whether you're going to underscore private props or
    // | not, don't leave it both
    unsigned long _currentMillis = 0;
    unsigned long _nextChange = 0;
    const long _noteDuration = 100;
    const long _pauseBetweenNotes = 100;
    boolean _notePlaying = false;
    int _activeNote = 0;
    boolean _playComplete = false;

    void _playMelody();
    void _playMelodyWithoutDelay();

public:
    TunePlayer(uint8_t speakerPin);

    // * Play a tune with delays
    void playMelody(TUNES name);

    // * Tools for playing a tune without delays
    void setActiveMelody(TUNES name);
    void playMelodyWithoutDelay();
    void stopPlaying();
    void reset();

    boolean playingMelody = false;
    TUNES currentMelody = TUNE_READY; // * consider a rename
};
#endif