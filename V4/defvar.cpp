#include "defvar.h"

unsigned char curP;
unsigned char malSelected;
unsigned char station[30];
signed char result;
unsigned char loc[numMal];
unsigned char route[numMal];
unsigned char routePos[numMal];
unsigned char status[numMal];
signed char total;
unsigned char flag, dest, change, ready, leave, sim, toggle, enter;
unsigned char toss_delay, sel_delay, move_delay;
unsigned char yut[numMal];
unsigned char time1, time2, time3, time4, time5, time6, time7;
unsigned char playMode;

char route0[26] = { 20,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,30,30,30,30,30 };
char route1[18] = { 4,5,21,22,23,24,25,15,16,17,18,19,20,30,30,30,30,30 };
char route2[13] = { 9,10,26,27,23,28,29,20,30,30,30,30,30 };
char route3[10] = { 22,23,28,29,20,30,30,30,30,30 };
char route4[7]  = { 1,20,30,30,30,30,30 };

