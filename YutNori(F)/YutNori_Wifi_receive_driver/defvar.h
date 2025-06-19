//defVar.h -- Contains defines and global variables
 #pragma once
 #define SEED 14132                                      //Random generator seed   //14132
 #define AI 0                                            //Assign AI 
     //(0: 1P, 1: 2P, 2: none)
 #define numMal 8                                        //Total number of mals
 
 unsigned char curP;                             //Player in turn
 #define P1 0                                            //Player labels
 #define P2 1
 
 unsigned char malSelected;                      //Selected mal
 unsigned char station[30];                      //Station occupancy
 #define P11 0                                           //Station occupancy status
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
 
 signed char result;                                     //Toss result
 #define BACKDO -1                                       //Toss result values
 #define DO 1
 #define GAE 2
 #define GEOL 3
 #define YUT 4
 #define MO 5
 //Mal properties:
 unsigned char loc[numMal];                      //Mal location
 unsigned char route[numMal];            //Mal route
 unsigned char routePos[numMal];         //Route position
 unsigned char status[numMal];           //Mal status
 
 //Mal status:
 #define SINGLE 0                                        //On board without ub
 #define DOUBLE 1                                        //On board with ub
 #define TRIPLE 2
 #define QUAD 3
 #define SCORE 4                                         //Scored
 
 signed char total;
 unsigned char flag, dest, change, ready, leave, sim, toggle, enter, toss_delay, sel_delay, move_delay;
 //Various state flags
 #define OFF 0                                           //Flags
 #define ON 1
 
 unsigned char yut[numMal];                      //Individual yut toss result
 #define DOWN 0                                          //Yut toss value
 #define UP 1
 //Timeout counters
 unsigned char time1, time2, time3, time4, time5, time6, time7;
 #define t1 2                                            //Refresh rate
 #define t2 250                                          //Blink rate
 #define t3 250                                          //Extender
 #define t4 6                                            //Delay rate
 #define t5 2                                            //Delay rate
 #define t6 3                                            //Move rate
 #define t7 2                                            //Debounce rate
 
 unsigned char playMode;                         //Current play mode
 #define TOSS 0                                          //Play modes
 #define SELECT 1
 #define MOVE 2
 
 //5 possible mal routes
 char route0[26] = { 20,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,30,30,30,30,30 };
 char route1[18] = { 4,5,21,22,23,24,25,15,16,17,18,19,20,30,30,30,30,30 };
 char route2[13] = { 9,10,26,27,23,28,29,20,30,30,30,30,30 };
 char route3[10] = { 22,23,28,29,20,30,30,30,30,30 };
 char route4[7] = { 1,20,30,30,30,30,30 };


// textlcd.c에서 정의된 attempt 변수에 대한 extern 선언
// main.c에서 attempt 값을 사용하기 위함
extern int attempt;