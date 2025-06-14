#ifndef TOSS_H
#define TOSS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "defvar.h"   // for yut[], result, sim, ready, sel_delay, time5, t5, playMode
#include <QDebug>    // qDebug() 를 사용하려면 필요

// 난수 시드 초기화 함수
static inline void initToss() {
    srand((unsigned)time(nullptr));
}

// Call this once per toss:
//   srand(SEED);

static inline void toss(void) {
    // 사람 차례에만 ENTER 또는 스페이스로 던지기
    if (ready == ON) {
        // getchar() 대신 하기 싫으면 스페이스바로 keyPressEvent 에서 던집니다.
        sim = ON;
        ready = OFF;
    }

    if (sim == ON) {
        // 4개의 윷, 앞면 확률 0.46
        for (char i = 0; i < 4; i++) {
            int num = rand() % 100;
            yut[i] = (num < 46 ? UP : DOWN);
        }

        // 결과 합산
        result = yut[0] + yut[1] + yut[2] + yut[3];
        qDebug() << "[toss] result sum:" << result;
        if (result == 0)                    result = MO;
        else if (yut[0] == UP && result == DO)
            result = BACKDO;

        // 다음 단계로
        sim = OFF;
        playMode = SELECT;
        sel_delay = ON;
        time5 = t5;
    }
}

#endif // TOSS_H
