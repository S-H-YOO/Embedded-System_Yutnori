#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include "buttondrv.h"
#include "fnd_control.h"
#include "textlcd.h"  // attempt 관련 함수

#define BUTTON_DEVICE "/dev/input/event5"

// 점(.) 이동
void move_cursor_left() {
    do {
        cursor = (cursor - 1 + 6) % 6;
    } while (fnd_digits[cursor] < 0);
    update_fnd_display();
}

// 선택된 숫자 삭제 및 왼쪽으로 shift
void delete_and_shift() {
    if (fnd_digits[cursor] < 0) return;

    for (int i = cursor; i > 0; i--) {
        fnd_digits[i] = fnd_digits[i - 1];
    }
    fnd_digits[0] = -1;

    for (int i = cursor; i < 6; i++) {
        if (fnd_digits[i] >= 0) {
            cursor = i;
            break;
        }
    }

    update_fnd_display();
}

// 단일 버튼 이벤트 처리 (main 루프 내에서 반복 호출)
void run_button_handler_once(int fd) {
    struct input_event ev;

    if (read(fd, &ev, sizeof(ev)) != sizeof(ev)) return;

    printf("EV: type=%d code=%d value=%d\n", ev.type, ev.code, ev.value);

    if (ev.type == EV_KEY && ev.value == 1) {
        if (ev.code == KEY_VOLUMEUP) {
            move_cursor_left();
        }
        else if (ev.code == KEY_BACK) {
            delete_and_shift();
        }
        else if (ev.code == KEY_VOLUMEDOWN) {
            increment_attempt();  // VolDn 누르면 attempt++
        }
    }
}
