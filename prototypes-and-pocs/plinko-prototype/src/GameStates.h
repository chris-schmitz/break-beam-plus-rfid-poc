#ifndef GAME_STATES
#define GAME_STATES

// | See if you can change the underlying type of the enums to a byte
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
#endif