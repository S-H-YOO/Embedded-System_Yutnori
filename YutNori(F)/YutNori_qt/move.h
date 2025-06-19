#pragma once
// move.h -- Moves the selected mal one step per QTimer tick
#ifndef MOVE_H
#define MOVE_H
#include <QDebug>

#include "defvar.h"
#include "movemal.h"
#include "moveresult.h"

/**
 * Advances the selected mal by exactly one step each time it's called.
 * Pacing is handled externally by QTimer in the Qt UI layer.
 */
void updateLEDFromMalStatus() {
    uint8_t ledMask = 0;

    if (status[P11] != SCORE) ledMask |= (1 << 0);
    if (status[P12] != SCORE) ledMask |= (1 << 1);
    if (status[P21] != SCORE) ledMask |= (1 << 4);
    if (status[P22] != SCORE) ledMask |= (1 << 5);

    QFile device("/dev/periled");
    if (!device.open(QIODevice::WriteOnly)) {
        qWarning() << "[LED] Failed to open /dev/periled";
        return;
    }

    char out = static_cast<char>(ledMask);
    device.write(&out, 1);
    device.close();
}


static inline void moveStep() {
    qDebug() << "[moveStep] enter: playMode=" << playMode
             << " malSelected=" << malSelected
             << " total=" << total
             << " leave=" << leave;

    if (leave == OFF) {
        // Departure: clear old station, store remaining steps
        station[loc[malSelected]] = EMPTY;
        leave = ON;
        total = result;
        return;  // next call will perform the first move
    }

    // Already departed: perform one step per invocation
    if (total > 0) {
        // Forward move
        if (curP == P1 && status[malSelected] == DOUBLE) {
            routePos[P11]++; moveMal(P11);
            routePos[P12]++; moveMal(P12);
        }
        else if (curP == P2 && status[malSelected] == DOUBLE) {
            routePos[P21]++; moveMal(P21);
            routePos[P22]++; moveMal(P22);
        }
        else {
            routePos[malSelected]++;
            moveMal(malSelected);
        }
        total--;        // one step consumed
    }
    else if (total < 0) {
        // Backdo move
        if (curP == P1 && status[malSelected] == DOUBLE) {
            routePos[P11]--; moveMal(P11);
            routePos[P12]--; moveMal(P12);
        }
        else if (curP == P2 && status[malSelected] == DOUBLE) {
            routePos[P21]--; moveMal(P21);
            routePos[P22]--; moveMal(P22);
        }
        else {
            routePos[malSelected]--;
            moveMal(malSelected);
        }
        total++;
    }
    else {
        // total == 0: reached destination
        if (curP == P1 && status[malSelected] == DOUBLE) {
            updateRoute(P11); moveMal(P11);
            updateRoute(P12); moveMal(P12);
        }
        else if (curP == P2 && status[malSelected] == DOUBLE) {
            updateRoute(P21); moveMal(P21);
            updateRoute(P22); moveMal(P22);
        }
        else {
            updateRoute(malSelected);
            moveMal(malSelected);
        }
        // Post-move checks
        checkScore();    // win or scoring logic
        updateLEDFromMalStatus();
	checkStation();  // stacking/capture logic
        leave = OFF;    // reset for next turn
    }
}

#endif // MOVE_H

