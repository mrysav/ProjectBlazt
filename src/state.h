#ifndef __STATE_H__
#define __STATE_H__

typedef struct GameState {
    void (*processInput) (int* keys);
    void (*updateDisplay) ();
} GameState;

#endif
