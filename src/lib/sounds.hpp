#pragma once
#include <raylib.h>
#define MAX_SOUNDS 10


class Soundman {
    private:
        Sound soundArray[MAX_SOUNDS] = { 0 };
        int currentSound;

    public:
    Soundman() {
        soundArray[0] = LoadSound("res/sounds/hit-tree.mp3");

        for (int i = 1; i < MAX_SOUNDS; i++)
        {
            soundArray[i] = LoadSoundAlias(soundArray[0]);        // Load an alias of the sound into slots 1-9. These do not own the sound data, but can be played
        }
        currentSound = 0;
    }
    void playHitSound() {
        PlaySound(soundArray[currentSound]);            // play the next open sound slot
        currentSound++;                                 // increment the sound slot
        if (currentSound >= MAX_SOUNDS)                 // if the sound slot is out of bounds, go back to 0
            currentSound = 0;
    }

    void unLoad() {

    }
};