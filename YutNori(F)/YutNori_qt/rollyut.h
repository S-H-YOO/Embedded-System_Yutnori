#pragma once
#ifndef ROLLYUT_H
#define ROLLYUT_H

#include <cstdlib>
#include <ctime>
#include "defvar.h"

// 매크로 이름과 겹치는 심볼 해제
#undef DO
#undef GAE
#undef GEOL
#undef YUT
#undef MO

// Yut 결과 타입 정의
enum class YutResult : int {
    DO   = 1,
    GAE  = 2,
    GEOL = 3,
    YUT  = 4,
    MO   = 5
};

// 헤더 온리 구현
static inline YutResult rollYut() {
    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        seeded = true;
    }
    int r = std::rand() % 5 + 1;
    return static_cast<YutResult>(r);
}

#endif // ROLLYUT_H
