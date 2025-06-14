#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "textlcddrv.h"
#include "textlcd.h"
#include "fnd_control.h"

#define TEXT_LCD_DEVICE "/dev/peritextlcd"

int player = 1;     // 현재 플레이어 (1 or 2)
int attempt = 1;    // 시도 횟수

// LCD에 Player / Attempt 표시
void update_textlcd_display() {
    int fd = open(TEXT_LCD_DEVICE, O_WRONLY);
    if (fd < 0) {
        perror("Text LCD open 실패");
        return;
    }

    stTextLCD lcd;
    memset(&lcd, 0, sizeof(lcd));

    snprintf(lcd.TextData[0], LINE_BUFF_NUM, "Player %d", player);
    snprintf(lcd.TextData[1], LINE_BUFF_NUM, "Attempt : %d", attempt);

    lcd.cmd = CMD_WRITE_STRING;
    lcd.cmdData = CMD_DATA_WRITE_BOTH_LINE;

    write(fd, &lcd, sizeof(lcd));
    close(fd);
}

// FND 비어 있고 attempt == 0이면 플레이어 전환
void check_and_toggle_player() {
    int all_cleared = 1;
    for (int i = 0; i < 6; i++) {
        if (fnd_digits[i] >= 0) {
            all_cleared = 0;
            break;
        }
    }

    if (all_cleared && attempt == 0) {
        player = 3 - player;   // 플레이어 전환
        attempt = 1;           // 새 플레이어 시도 초기화

        init_fnd_digits();
        update_fnd_display();
        update_textlcd_display();
    }
}

// 시도 횟수 증가
void increment_attempt() {
    attempt++;
    update_textlcd_display();
}

// 시도 횟수 감소 (0 이하로 내려가지 않게 보호)
void decrement_attempt() {
    if (attempt > 0)
        attempt--;
    update_textlcd_display();
}
