#pragma once
#define SEED 14132
#define AI 0
#define numMal 8

extern unsigned char curP;
#define P1 0
#define P2 1

extern unsigned char malSelected;
extern unsigned char station[30];

#define P11 0
#define P12 1
#define P13 2
#define P14 3
#define P21 4
#define P22 5
#define P23 6
#define P24 7
#define EMPTY 8
#define DOUBLE1 9
#define TRIPLE1 10
#define QUAD1 11
#define DOUBLE2 12
#define TRIPLE2 13
#define QUAD2 14

extern signed char result;
#define BACKDO -1
#define DO 1
#define GAE 2
#define GEOL 3
#define YUT 4
#define MO 5

extern unsigned char loc[numMal];
extern unsigned char route[numMal];
extern unsigned char routePos[numMal];
extern unsigned char status[numMal];

#define SINGLE 0
#define DOUBLE 1
#define TRIPLE 2
#define QUAD 3
#define SCORE 4

extern signed char total;
extern unsigned char flag, dest, change, ready, leave, sim, toggle, enter, toss_delay, sel_delay, move_delay;

#define OFF 0
#define ON 1

extern unsigned char yut[numMal];
#define DOWN 0
#define UP 1

extern unsigned char time1, time2, time3, time4, time5, time6, time7;

#define t1 2
#define t2 250
#define t3 250
#define t4 6
#define t5 2
#define t6 3
#define t7 2

extern unsigned char playMode;
#define TOSS 0
#define SELECT 1
#define MOVE 2

extern char route0[26];
extern char route1[18];
extern char route2[13];
extern char route3[10];
extern char route4[7];

