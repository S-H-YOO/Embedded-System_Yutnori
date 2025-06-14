#ifndef TEXTLCD_H
#define TEXTLCD_H

extern int attempt;

// Player / Attempt 정보 갱신 → Text LCD에 출력
void update_textlcd_display(void);

// FND가 비어있고 attempt == 0일 때만 player 전환
void check_and_toggle_player(void);

// 시도 횟수 증가 (Attempt++)
void increment_attempt(void);

// 시도 횟수 감소 (Attempt--)
void decrement_attempt(void);

#endif // TEXTLCD_H
