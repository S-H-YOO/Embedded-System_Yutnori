#include "fnd_logic.h"
#include "fnddrv.h"
#include <linux/input.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int fnd_digits[6] = {1, 2, 3, 4, 5, 6};
int valid[6] = {1, 1, 1, 1, 1, 1};
int cursor = 5;

void init_fnd_digits() {
    int i;
    for (i = 0; i < 6; i++) {
        fnd_digits[i] = i + 1;
        valid[i] = 1;
    }
    cursor = 5;
    update_fnd_display();
}

void move_cursor_left() {
    do {
        cursor = (cursor - 1 + 6) % 6;
    } while (!valid[cursor]);
    update_fnd_display();
}

void delete_digit_at_cursor() {
    if (!valid[cursor]) return;

    valid[cursor] = 0;
    for (int i = cursor; i > 0; i--) {
        fnd_digits[i] = fnd_digits[i - 1];
        valid[i] = valid[i - 1];
    }
    valid[0] = 0;
    update_fnd_display();
}

void update_fnd_display() {
    int fd = open("/dev/perifnd", O_WRONLY);
    if (fd < 0) {
        perror("FND 디바이스 열기 실패");
        return;
    }

    stFndWriteForm form;
    memset(&form, 0, sizeof(stFndWriteForm));

    for (int i = 0; i < 6; i++) {
        if (valid[i]) {
            form.DataValid[i] = 1;
            form.DataNumeric[i] = fnd_digits[i];
        }
    }

    form.DataDot[cursor] = 1;

    write(fd, &form, sizeof(form));
    close(fd);
}
