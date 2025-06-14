#ifndef FND_BTN_H
#define FND_BTN_H

void init_fnd_digits();                  // 초기 FND 배열 설정
void move_cursor_left();                 // 커서 이동
void delete_digit_at_cursor();          // 현재 커서 위치 숫자 삭제
void update_fnd_display();              // 현재 상태 FND에 출력

#endif
