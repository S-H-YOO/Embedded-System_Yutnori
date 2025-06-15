#ifndef INITIALIZE_CONSOLE_H
#define INITIALIZE_CONSOLE_H

#include <stdlib.h>
#include "defvar.h"  // for all game variables and SEED

/**
 * initialize(): Sets up all game variables for console environment.
 * Removes hardware-specific setup (GPIO, timers, interrupts).
 */
static inline void initialize(void) {
    // Initialize timer counters
    time1 = t1;
    time2 = t2;
    time3 = t3;
    time4 = t4;
    time5 = t5;
    time6 = t6;
    time7 = t7;
    
    {
        FILE *leddev = fopen("/dev/periled", "w");
        if (leddev) {
            char init = (char)0xCC;
            fwrite(&init, 1, 1, leddev);
            fclose(leddev);
        }
    }
    // Initialize all stations to empty
    for (int y = 0; y < 30; y++) {
        station[y] = EMPTY;
    }

    // Initialize all pieces
    for (int j = 0; j < numMal; j++) {
        loc[j] = 0;         // Home position
        route[j] = 0;         // Default route
        routePos[j] = 0;         // Start of route
        status[j] = SINGLE;    // Single-piece status
    }

    // Initialize game flags and variables
    flag = 0;
    curP = P1;
    playMode = TOSS;
    total = 0;
    dest = 0;
    ready = OFF;
    change = ON;
    leave = OFF;
    sim = OFF;
    playMode = TOSS;
    malSelected = P11;
    curP = P1;
    sel_delay = OFF;
    move_delay = OFF;
    toss_delay = ON;

    // Seed the random number generator for reproducibility
    srand(SEED);
}

#endif // INITIALIZE_CONSOLE_H
#pragma once
