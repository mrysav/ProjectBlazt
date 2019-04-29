#ifndef __STATE_H__
#define __STATE_H__

typedef enum State {
    MENU,
    GAME,
    EXIT
} State;

typedef struct GameState {
    void (*loadResources) ();
    State (*processInput) (unsigned char* keys);
    void (*updateDisplay) ();
    void (*unloadResources) ();
} GameState;

#endif
