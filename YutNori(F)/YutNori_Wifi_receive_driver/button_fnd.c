#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "buttondrv.h"
#include "fnd_control.h"
#include "textlcd.h"
#include "ipc_messages.h"

#define BUTTON_DEVICE "/dev/input/event5"

int selected_fnd_num = -1;
int selected_mal_idx = -1;

void move_cursor_left() {
    do {
        cursor = (cursor - 1 + 6) % 6;
    } while (fnd_digits[cursor] < 0);
    update_fnd_display();
}

void delete_and_shift() {
    if (fnd_digits[cursor] < 0) return;

    selected_fnd_num = fnd_digits[cursor];

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

void run_button_handler_once(int fd) {
    struct input_event ev;

    if (read(fd, &ev, sizeof(ev)) != sizeof(ev)) return;

    printf("EV: type=%d code=%d value=%d\n", ev.type, ev.code, ev.value);

    if (ev.type == EV_KEY && ev.value == 1) {
        if (ev.code == KEY_VOLUMEUP) {
            move_cursor_left();
        }
        else if (ev.code == KEY_BACK) {
            if (fnd_digits[cursor] >= 0) {
                selected_fnd_num = fnd_digits[cursor];
                delete_and_shift();

                // yut_result 메시지 전송
                key_t key = ftok(MSG_KEY_PATH, MSG_KEY_ID);
                if (key == -1) {
                    perror("ftok 실패");
                    return;
                }

                int msqid = msgget(key, IPC_CREAT | 0666);
                if (msqid == -1) {
                    perror("msgget 실패");
                    return;
                }

                MyMsgBuf msg;
                msg.mtype = MSG_TYPE_YUT;
                msg.yut_result = selected_fnd_num;

                if (msgsnd(msqid, &msg, sizeof(msg.yut_result), 0) == -1) {
                    perror("msgsnd 실패");
                } else {
                    printf("▶ 선택된 숫자 %d → 메시지 큐 전송 완료\n", selected_fnd_num);
                }

                // 선택 유지
            }
        }
        else if (ev.code == KEY_VOLUMEDOWN) {
            // 기존의 attempt 증가 삭제
            // 새로운 역할: player 전환 조건을 위한 flag 설정
            //voldn_pressed = 1;  // 외부 전역 변수로 선언 필요
        }
        else if (ev.code == KEY_SEARCH || ev.code == KEY_MENU) {
            printf("▶ DEBUG: selected_fnd_num = %d\n", selected_fnd_num);

            if (selected_fnd_num >= 0) {
                selected_mal_idx = (ev.code == KEY_SEARCH) ? 0 : 1;

                key_t key = ftok(MSG_KEY_PATH, MSG_KEY_ID);
                if (key == -1) {
                    perror("ftok 실패");
                    return;
                }

                int msqid = msgget(key, IPC_CREAT | 0666);
                if (msqid == -1) {
                    perror("msgget 실패");
                    return;
                }

                MoveMalMsgBuf msg;
                msg.mtype = MSG_TYPE_MOVE_MAL;
                msg.mal_id = selected_mal_idx;
                msg.move_steps = selected_fnd_num;

                if (msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                    perror("말 이동 msgsnd 실패");
                } else {
                    printf("▶ IPC 전송: 말 %d → %d칸\n", msg.mal_id, msg.move_steps);
                }

                selected_fnd_num = -1;
                selected_mal_idx = -1;
            } else {
                printf("⚠️ 이동할 숫자가 아직 선택되지 않았습니다.\n");
            }
        }
    }
}
