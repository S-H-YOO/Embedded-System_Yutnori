#pragma once
#ifndef MOVEMAL_H
#define MOVEMAL_H

#include "defvar.h"   // for route[], routePos[], loc[], routeX arrays, dest
#include "turncontrol.h"

//------------------------------------------------------------------------------
// Move mal(s) along the route
//------------------------------------------------------------------------------
// mal: 인덱스(0~7) 로 표시된 말(piece)
// route[mal] 에 따라 해당 routeX 배열에서 현재 위치(routePos[mal])를
// 읽어서 loc[mal] 에 저장합니다.
static inline void moveMal(char mal) {
    ////////////////////////////////////////////////////////////////////// protection
    if (status[mal] == SCORE) {						//
        qDebug() << "[moveMal] mal already scored. Ignoring move.";	//
        return;								//
    }									//
    //////////////////////////////////////////////////////////////////////

    switch (route[mal]) {
    case 0:  loc[mal] = route0[routePos[mal]]; break;
    case 1:  loc[mal] = route1[routePos[mal]]; break;
    case 2:  loc[mal] = route2[routePos[mal]]; break;
    case 3:  loc[mal] = route3[routePos[mal]]; break;
    case 4:  loc[mal] = route4[routePos[mal]]; break;
    default: /* no change */                break;
    }
}


//------------------------------------------------------------------------------
// Perform route changes at forks & backdo spots
//------------------------------------------------------------------------------
// mal: 말 인덱스
// routePos[mal] 이 가리키는 위치(loc[mal]) 가
// 지름길 입구나 백도 복귀 지점이면 route[] 와 routePos[] 를 재설정합니다.
static inline void updateRoute(char mal) {
    switch (route[mal]) {
    case 0:
        if (loc[mal] == 5) { route[mal] = 1; routePos[mal] = 1; }
        else if (loc[mal] == 10) { route[mal] = 2; routePos[mal] = 1; }
        else if (loc[mal] == 20) { route[mal] = 4; routePos[mal] = 1; }
        break;
    case 1:
        if (loc[mal] == 23) { route[mal] = 3; routePos[mal] = 1; }
        else if (loc[mal] == 4) { route[mal] = 0; routePos[mal] = 4; }
        break;
    case 2:
        if (loc[mal] == 9) { route[mal] = 0; routePos[mal] = 9; }
        break;
    case 3:
        if (loc[mal] == 22) { route[mal] = 1; routePos[mal] = 3; }
        break;
    case 4:
        if (loc[mal] == 1) { route[mal] = 0; routePos[mal] = 1; }
        break;
    default:
        break;
    }
}


//------------------------------------------------------------------------------
// Predict destination for AI or lookahead
//------------------------------------------------------------------------------
// mal: 말 인덱스, space: 앞으로 이동할 칸 수
// return: 이동 후 도착할 station 번호 (0은 홈 복귀 혹은 범위 초과)
static inline signed char checkDest(char mal, signed char space) {
    signed char d = 0;
    switch (route[mal]) {
    case 0:
        d = (route0[routePos[mal] + space] > 0
                 ? route0[routePos[mal] + space]
                 : 0);
        break;
    case 1:
        d = (route1[routePos[mal] + space] > 0
                 ? route1[routePos[mal] + space]
                 : 0);
        break;
    case 2:
        d = (route2[routePos[mal] + space] > 0
                 ? route2[routePos[mal] + space]
                 : 0);
        break;
    case 3:
        d = (route3[routePos[mal] + space] > 0
                 ? route3[routePos[mal] + space]
                 : 0);
        break;
    case 4:
        d = (route4[routePos[mal] + space] > 0
                 ? route4[routePos[mal] + space]
                 : 0);
        break;
    default:
        d = 0;
        break;
    }
    return d;
}

#endif // MOVEMAL_H
