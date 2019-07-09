#include "TunePlayer.h"

TunePlayer::TunePlayer(uint8_t pin)
{
    speakerPin = pin;
}

// * Shave and a haircut
int TunePlayer::length_shaveAndAHairCut = 9;
int TunePlayer::melody_shaveAndAHairCut[] = {
    NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4, 0};
int TunePlayer::durations_shaveAndAHairCut[] = {
    4, 8, 8, 4, 4, 4, 4, 4, 4};

// * READY
int TunePlayer::length_READY = 8;
int TunePlayer::melody_READY[] = {
    NOTE_C3, NOTE_C3, 0, NOTE_C4, NOTE_C4, 0, NOTE_C3, NOTE_C3};
int TunePlayer::durations_READY[] = {
    16, 16, 8, 16, 16, 8, 16, 16};

// * PROCESSING
int TunePlayer::length_PROCESSING = 8;
int TunePlayer::melody_PROCESSING[] = {
    NOTE_C3, NOTE_D3, NOTE_E3, NOTE_D3, NOTE_C3, NOTE_D3, NOTE_E3, NOTE_D3};
int TunePlayer::durations_PROCESSING[] = {
    8, 8, 8, 8, 8, 8, 8, 8};

// * COMPLETE
int TunePlayer::length_COMPLETE = 4;
int TunePlayer::melody_COMPLETE[] = {
    NOTE_C4, NOTE_C4, NOTE_C4, NOTE_C4};
int TunePlayer::durations_COMPLETE[] = {
    8, 8, 8, 2};

// * IDLE
int TunePlayer::length_IDLE = 14;
int TunePlayer::melody_IDLE[] = {
    NOTE_C3, NOTE_C3, NOTE_C3, NOTE_E3, NOTE_C3, NOTE_F3, NOTE_G3, 0, NOTE_G3, NOTE_G3, NOTE_F3, NOTE_D3, NOTE_E3, NOTE_C3};
int TunePlayer::durations_IDLE[] = {
    8, 4, 8, 8, 8, 8, 4, 2, 8, 4, 8, 8, 8, 8};

/**
 * * Play a given melody per the passed in enum.
 * * Params:
 * * MELODY name The MELODY enum value
 */
void TunePlayer::playMelody(TUNES name)
{
    setActiveMelody(name);
    _playMelody();
}

/**
 * * Plays the set melody without using `delay()`.
 * ! Note that you need to use `setActiveMelody()` to, you know, set the active melody
 * ! before you call this method.
 */
void TunePlayer::playMelodyWithoutDelay()
{
    if (playingMelody == true)
    {
        _currentMillis = millis();
        _playMelodyWithoutDelay();
    }
}

/**
 * * Stop the play of the current melody,
 * * Note that this essentially acts as a pause, if you fire `playMelodyWithoutDelay()` again
 * * without resetting or setting a new melody the player will pick up where it left off.
 * ^ Theoretcially, I haven't actually tried this out and current me can't be bothered to test yet ;P
 */
void TunePlayer::stopPlaying()
{
    playingMelody = false;
}

/**
 * * Reset the player state so it's ready to play a new melody.
 */
void TunePlayer::reset()
{
    _activeNote = 0;
    _playComplete = false;
    activeLength = 0;
    // ? anything else to reset?
}

/**
 * * The method for actually playing the selected melody.
 * ! note that the melody desired should be set in the
 * ! active pointer properties before this method is called
 */
void TunePlayer::_playMelody()
{
    // * pulled from the toneMelody arduino example
    for (int thisNote = 0; thisNote < activeLength; thisNote++)
    {

        int activeDuration = activeDurations[thisNote];
        int noteDuration = 1000 / activeDurations[thisNote];
        int noteFrequency = activeMelody[thisNote];

        // * leaving in for troubleshooting
        // Serial.println(thisNote);
        // Serial.println(activeDuration);
        // Serial.println(noteDuration);
        // Serial.println(noteFrequency);

        tone(speakerPin, noteFrequency, noteDuration);

        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);

        noTone(speakerPin);
    }
}

void TunePlayer::_playMelodyWithoutDelay()
{
    // * Not ready for change, kick out early
    if (_currentMillis < _nextChange)
    {
        return;
    }

    unsigned long millisTillNextChange = 0;

    if (_notePlaying)
    {
        millisTillNextChange = _pauseBetweenNotes;
        noTone(speakerPin);
        if (_playComplete)
        {

            playingMelody = false;
        }
    }
    else
    {
        millisTillNextChange = 1000 / activeDurations[_activeNote];

        if (activeMelody[_activeNote] != 0)
        {
            int note = activeMelody[_activeNote];
            tone(speakerPin, note);
        }
        else
        {
            noTone(speakerPin);
        }

        _activeNote++;
        if (_activeNote >= activeLength)
        {
            _activeNote = 0;
            _playComplete = true;
        }
    }

    _notePlaying = !_notePlaying;
    _nextChange = _currentMillis + millisTillNextChange;
}

void TunePlayer::setActiveMelody(TUNES name)
{
    // ? PASTE ME: is this the right spot for this??
    // * PASTE ME: naming wise I don't think it is, but logic wise it might be
    // * PRESENT ME: HAHAHAHA IT'S WORKING AND I COULD CARE LESS AT THE MOMENT! LET FUTURE ME WORRY ABOUT IT!
    playingMelody = true;

    currentMelody = name;
    switch (name)
    {
    case TUNE_SHAVE_AND_A_HAIRCUT:
        activeLength = length_shaveAndAHairCut;
        activeMelody = melody_shaveAndAHairCut;
        activeDurations = durations_shaveAndAHairCut;
        break;
    case TUNE_READY:
        activeLength = length_READY;
        activeMelody = melody_READY;
        activeDurations = durations_READY;
        break;
    case TUNE_PROCESSING:
        activeLength = length_PROCESSING;
        activeMelody = melody_PROCESSING;
        activeDurations = durations_PROCESSING;
        break;
    case TUNE_COMPLETE:
        activeLength = length_COMPLETE;
        activeMelody = melody_COMPLETE;
        activeDurations = durations_COMPLETE;
        break;
    case TUNE_IDLE:
        activeLength = length_IDLE;
        activeMelody = melody_IDLE;
        activeDurations = durations_IDLE;
        break;
    }
}