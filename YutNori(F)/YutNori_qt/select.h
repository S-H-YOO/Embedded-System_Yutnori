// select.h -- Enables the player to select which mal to move (console-only, no AI)
#include <stdio.h>
#include "turncontrol.h"
void select(void) {
    // --- BACKDO 특수 처리 ---
    if (curP == P1 && result == BACKDO && loc[P11] == 0 && loc[P12] == 0) {
        endTurn();
        return;
    }
    if (curP == P2 && result == BACKDO && loc[P21] == 0 && loc[P22] == 0) {
        endTurn();
        return;
    }
    if (curP == P1 && result == BACKDO && status[P11] == SCORE && loc[P12] == 0) {
        endTurn();
        return;
    }
    if (curP == P1 && result == BACKDO && status[P12] == SCORE && loc[P11] == 0) {
        endTurn();
        return;
    }
    if (curP == P2 && result == BACKDO && status[P21] == SCORE && loc[P22] == 0) {
        endTurn();
        return;
    }
    if (curP == P2 && result == BACKDO && status[P22] == SCORE && loc[P21] == 0) {
        endTurn();
        return;
    }
    // 한 쪽 말만 뒤로 보낼 수 있을 때 자동 선택
    if (curP == P1 && result == BACKDO && loc[P11] == 0) {
        malSelected = P12; change = OFF; return;
    }
    if (curP == P1 && result == BACKDO && loc[P12] == 0) {
        malSelected = P11; change = OFF; return;
    }
    if (curP == P2 && result == BACKDO && loc[P21] == 0) {
        malSelected = P22; change = OFF; return;
    }
    if (curP == P2 && result == BACKDO && loc[P22] == 0) {
        malSelected = P21; change = OFF; return;
    }

    // --- 사람 차례: 콘솔 입력 처리 ---
    // 현재 선택된 말 안내
    printf("Player %d  말 %d 선택됨\n",
           curP + 1, (malSelected % 2) + 1);
    printf("토글: 't' 키, 확정: Enter\n");

    int c = getchar();
    // 토글
    if (c == 't' || c == 'T') {
        if (curP == P1) {
            malSelected = (malSelected == P11 ? P12 : P11);
        }
        else {
            malSelected = (malSelected == P21 ? P22 : P21);
        }
        return;  // 말 변경 후 다시 select() 호출될 때까지 대기
    }
    // 확정
    if (c == '\n') {
        if (status[malSelected] == SCORE) {
        //    printf("이 말은 이미 도착했습니다. 다른 말을 선택하세요.\n");
        //    return;  // 잘못된 말 선택 → 다시 선택 대기
	    int alt = -1;
            if (curP == P1) {
                if (malSelected == P11 && status[P12] != SCORE) alt = P12;
                if (malSelected == P12 && status[P11] != SCORE) alt = P11;
            } else {
                if (malSelected == P21 && status[P22] != SCORE) alt = P22;
                if (malSelected == P22 && status[P21] != SCORE) alt = P21;
            }

            if (alt != -1) {
                malSelected = alt;
                printf("자동으로 남은 말로 전환했습니다.\n");
                return; // <= protection
	    } else {
                printf("두 말 모두 도착했습니다. 이동할 수 없습니다.\n");
                ////////////////////////////// for protection
		playMode = SELECT;          //
		//////////////////////////////
		return;
            }        

	}
	playMode = MOVE;
        move_delay = ON;
        time5 = t5;
        return;
    }
}
