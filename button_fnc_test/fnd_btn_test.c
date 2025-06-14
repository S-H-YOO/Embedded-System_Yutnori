#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include "buttondrv.h"
#include "fnddrv.h"

#define FND_DEVICE "/dev/perifnd"

static int fnd_digits[6] = {1, 2, 3, 4, 5, 6};
static int cursor = 5;

void update_fnd_display() {
    int fd = open(FND_DEVICE, O_WRONLY);
    if (fd < 0) {
        perror("FND 디바이스 열기 실패");
        return;
    }

    stFndWriteForm fnd;
    memset(&fnd, 0, sizeof(fnd));

    for (int i = 0; i < 6; i++) {
        if (fnd_digits[i] >= 0) {
            fnd.DataValid[i] = 1;
            fnd.DataNumeric[i] = fnd_digits[i];
            fnd.DataDot[i] = (i == cursor) ? 1 : 0;
        }
    }

    write(fd, &fnd, sizeof(fnd));
    close(fd);
}

void move_cursor_left() {
    do {
        cursor = (cursor - 1 + 6) % 6;
    } while (fnd_digits[cursor] < 0);
    update_fnd_display();
}

void delete_and_shift() {
    if (fnd_digits[cursor] < 0) return;

    // 삭제하고 왼쪽 값을 오른쪽으로 밀기
    for (int i = cursor; i > 0; i--) {
        fnd_digits[i] = fnd_digits[i - 1];
    }
    fnd_digits[0] = -1;

    // 커서가 가리키던 자리에 값이 사라졌으므로
    // 만약 해당 자리도 -1이면, 오른쪽 중 유효한 자리로 커서를 옮기자
    if (fnd_digits[cursor] < 0) {
        for (int i = cursor; i < 6; i++) {
            if (fnd_digits[i] >= 0) {
                cursor = i;
                break;
            }
        }
    }

    update_fnd_display();
}

int main() {
    int btn_fd;
    struct input_event ev;

    btn_fd = open(BUTTON_DEVICE, O_RDONLY);
    if (btn_fd < 0) {
        perror("버튼 디바이스 열기 실패");
        return -1;
    }

    printf("▶ FND 버튼 컨트롤 시작 (왼쪽 이동: HOME_KEY, 삭제: BACK_KEY)\n");
    update_fnd_display();

    while (1) {
        if (read(btn_fd, &ev, sizeof(ev)) != sizeof(ev)) continue;

        if (ev.type == EV_KEY && ev.value == 1) {
            if (ev.code == BUTTON_CODE_MOVE) {
                move_cursor_left();
            } else if (ev.code == BUTTON_CODE_DELETE) {
                delete_and_shift();
            }
        }
        usleep(10000);
    }

    close(btn_fd);
    return 0;
}
