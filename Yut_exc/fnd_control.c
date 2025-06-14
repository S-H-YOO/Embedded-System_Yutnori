#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "fnddrv.h"
#include "fnd_control.h"

#define FND_DEVICE "/dev/perifnd"

int fnd_digits[6] = {-1, -1, -1, -1, -1, -1};
int cursor = 5;

void init_fnd_digits() {
    for (int i = 0; i < 6; i++) fnd_digits[i] = -1;
    cursor = 5;
}

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

void insert_fnd_number(int number) {
    for (int i = 0; i < 5; i++) {
        fnd_digits[i] = fnd_digits[i + 1];
    }
    fnd_digits[5] = number;
    update_fnd_display();
}
