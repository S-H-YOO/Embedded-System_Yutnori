#pragma once
// moveResult.h -- Console‐only post‐move checks (no hardware blink loops)
#ifndef MOVERESULT_H
#define MOVERESULT_H

#include <stdio.h>
#include <stdlib.h>
#include "defvar.h"    // for curP, malSelected, station[], loc[], route[], routePos[], status[], etc.

//------------------------------------------------------------------------------
// Inspect the destination station and handle stacking, capturing, or moving on.
// Calls continueTurn() or endTurn() to advance game state.
//------------------------------------------------------------------------------
inline void checkStation(void) {
    // Player 1’s turn
    if (curP == P1) {
        // Own-piece stacking
        if ((malSelected == P11 && station[loc[P11]] == P12) ||
            (malSelected == P12 && station[loc[P12]] == P11)) {
            status[P11] = status[P12] = DOUBLE;
            station[loc[malSelected]] = DOUBLE1;
            if (total == YUT || total == MO) continueTurn();
            else                            endTurn();
            return;
        }

        // Capture opponent’s single or double piece(s)
        if (station[loc[malSelected]] == P21 ||
            station[loc[malSelected]] == P22 ||
            station[loc[malSelected]] == DOUBLE2) {
            // Reset any captured P2 pieces
            if (station[loc[malSelected]] == P21 || station[loc[malSelected]] == P22) {
                char victim = station[loc[malSelected]];
                loc[victim] = 0;
                route[victim] = 0;
                routePos[victim] = 0;
                status[victim] = SINGLE;
            }
            else { // DOUBLE2
                loc[P21] = loc[P22] = 0;
                route[P21] = route[P22] = 0;
                routePos[P21] = routePos[P22] = 0;
                status[P21] = status[P22] = SINGLE;
            }
            // Occupy now-empty station
            station[loc[malSelected]] = (status[malSelected] == DOUBLE ? DOUBLE1 : malSelected);
            continueTurn();
            return;
        }

        // Empty station: just occupy
        if (station[loc[malSelected]] == EMPTY) {
            station[loc[malSelected]] = (status[malSelected] == DOUBLE ? DOUBLE1 : malSelected);
            if (total == YUT || total == MO) continueTurn();
            else                            endTurn();
            return;
        }
    }
    // Player 2’s turn
    else {
        // Own-piece stacking
        if ((malSelected == P21 && station[loc[P21]] == P22) ||
            (malSelected == P22 && station[loc[P22]] == P21)) {
            status[P21] = status[P22] = DOUBLE;
            station[loc[malSelected]] = DOUBLE2;
            if (total == YUT || total == MO) continueTurn();
            else                            endTurn();
            return;
        }

        // Capture opponent’s single or double piece(s)
        if (station[loc[malSelected]] == P11 ||
            station[loc[malSelected]] == P12 ||
            station[loc[malSelected]] == DOUBLE1) {
            // Reset any captured P1 pieces
            if (station[loc[malSelected]] == P11 || station[loc[malSelected]] == P12) {
                char victim = station[loc[malSelected]];
                loc[victim] = 0;
                route[victim] = 0;
                routePos[victim] = 0;
                status[victim] = SINGLE;
            }
            else { // DOUBLE1
                loc[P11] = loc[P12] = 0;
                route[P11] = route[P12] = 0;
                routePos[P11] = routePos[P12] = 0;
                status[P11] = status[P12] = SINGLE;
            }
            station[loc[malSelected]] = (status[malSelected] == DOUBLE ? DOUBLE2 : malSelected);
            continueTurn();
            return;
        }

        // Empty station: just occupy
        if (station[loc[malSelected]] == EMPTY) {
            station[loc[malSelected]] = (status[malSelected] == DOUBLE ? DOUBLE2 : malSelected);
            if (total == YUT || total == MO) continueTurn();
            else                            endTurn();
            return;
        }
    }
}

//------------------------------------------------------------------------------
// End the current turn and switch to the next player.
//------------------------------------------------------------------------------
inline void endTurn(void) {
    playMode = TOSS;
    total = 0;
    leave = OFF;
    ready = ON;

    // Switch player and pick a valid default piece
    if (curP == P1) {
        curP = P2;
        if (status[P21] == SCORE) { malSelected = P22; change = OFF; }
        else if (status[P22] == SCORE) { malSelected = P21; change = OFF; }
        else { malSelected = P21; change = ON; }
    }
    else {
        curP = P1;
        if (status[P11] == SCORE) { malSelected = P12; change = OFF; }
        else if (status[P12] == SCORE) { malSelected = P11; change = OFF; }
        else { malSelected = P11; change = ON; }
    }

    // Prepare for next toss without hardware blink
    toss_delay = ON;
    time4 = t4;
}

//------------------------------------------------------------------------------
// Continue the current turn (after YUT or MO), same player.
//------------------------------------------------------------------------------
inline void continueTurn(void) {
    playMode = TOSS;
    total = 0;
    leave = OFF;
    ready = ON;

    // Keep same curP but reset selected piece
    if (curP == P1) {
        if (status[P11] == SCORE) { malSelected = P12; change = OFF; }
        else if (status[P12] == SCORE) { malSelected = P11; change = OFF; }
        else { malSelected = P11; change = ON; }
    }
    else {
        if (status[P21] == SCORE) { malSelected = P22; change = OFF; }
        else if (status[P22] == SCORE) { malSelected = P21; change = OFF; }
        else { malSelected = P21; change = ON; }
    }

    toss_delay = ON;
    time4 = t4;
}

//------------------------------------------------------------------------------
// Check if the selected mal has scored and handle win/next steps.
//------------------------------------------------------------------------------
inline void checkScore(void) {
    if (loc[malSelected] == 30) {
        // Mark single or double as scored
        if ((curP == P1 && status[malSelected] == DOUBLE)
            || (curP == P2 && status[malSelected] == DOUBLE)) {
            // Both pieces score
            status[(curP == P1 ? P11 : P21)] = SCORE;
            status[(curP == P1 ? P12 : P22)] = SCORE;
        }
        else {
            status[malSelected] = SCORE;
        }

        // Check for game end
        checkWin();

        if (total == YUT || total == MO) continueTurn();
        else                            endTurn();
    }
}

//------------------------------------------------------------------------------
// Check for a win and terminate the program with a message.
//------------------------------------------------------------------------------
inline void checkWin(void) {
    if (status[P11] == SCORE && status[P12] == SCORE) {
        printf("Player 1 wins!\n");
        exit(0);
    }
    if (status[P21] == SCORE && status[P22] == SCORE) {
        printf("Player 2 wins!\n");
        exit(0);
    }
}

#endif // MOVERESULT_H
