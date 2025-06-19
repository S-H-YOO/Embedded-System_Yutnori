#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "defvar.h"
#include "key_codes.h"
#include "ipc_messages.h"

#include "fnd_control.h"
#include "button_fnd.h"
#include "textlcd.h"

#define BUTTON_DEVICE "/dev/input/event5"
#define SERVER_PORT 9999

static int selected_fnd_num = -1;

int is_fnd_empty() {
    for (int i = 0; i < 6; i++) {
        if (fnd_digits[i] >= 0) return 0;
    }
    return 1;
}

int main() {
    int button_fd = -1;
    int server_fd = -1;
    int client_fd = -1;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    key_t msg_key = ftok(MSG_KEY_PATH, MSG_KEY_ID);
    if (msg_key == -1) {
        perror("ftok 실패");
        exit(EXIT_FAILURE);
    }

    int msgid = msgget(msg_key, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget 실패");
        exit(EXIT_FAILURE);
    }

    printf("▶ 시스템 초기화 중...\n");

    init_fnd_digits();
    update_fnd_display();
    update_textlcd_display();

    button_fd = open(BUTTON_DEVICE, O_RDONLY | O_NONBLOCK);
    if (button_fd < 0) {
        perror("버튼 디바이스 열기 실패");
        goto cleanup;
    }

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket 생성 실패");
        goto cleanup;
    }

    int optval = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind 실패");
        goto cleanup;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen 실패");
        goto cleanup;
    }

    printf("▶ 시스템 실행 중 - WiFi 수신 대기 (포트 %d)...\n", SERVER_PORT);

    while (1) {
        // 버튼 입력 처리
        struct input_event ev;
        if (read(button_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            if (ev.type == EV_KEY && ev.value == 1) {
                if (ev.code == KEY_VOLUMEUP) {
                    move_cursor_left();
                } else if (ev.code == KEY_BACK) {
                    if (fnd_digits[cursor] >= 0) {
                        selected_fnd_num = fnd_digits[cursor];
                        printf("▶ FND에서 %d 선택됨 (이동 칸 수).\n", selected_fnd_num);
                        delete_and_shift();
                    } else {
                        printf("▶ 커서 위치에 숫자가 없습니다.\n");
                    }
                } else if (ev.code == KEY_SEARCH || ev.code == KEY_MENU) {
                    if (selected_fnd_num != -1) {
                        int mal_to_move = (player == 1)
                            ? ((ev.code == KEY_SEARCH) ? P11 : P12)
                            : ((ev.code == KEY_SEARCH) ? P21 : P22);

                        printf("▶ 말 %d (플레이어 %d) 선택됨. %d 칸 이동 요청.\n",
                            mal_to_move, player, selected_fnd_num);

                        // IPC 메시지 전송
                        MoveMalMsgBuf move_msg;
                        move_msg.mtype = MSG_TYPE_MOVE_MAL;
                        move_msg.mal_id = mal_to_move;
                        move_msg.move_steps = selected_fnd_num;

                        if (msgsnd(msgid, &move_msg, sizeof(move_msg) - sizeof(long), 0) == -1) {
                            perror("말 이동 msgsnd 실패");
                        } else {
                            printf("▶ IPC 전송: 말 %d → %d칸 (플레이어 %d)\n",
                                   mal_to_move, selected_fnd_num, player);
                        }

                        selected_fnd_num = -1;
                    } else {
                        printf("▶ 먼저 이동할 숫자를 선택하세요.\n");
                    }
                } else if (ev.code == KEY_VOLUMEDOWN) {
                    voldn_pressed = 1;
                }
            }
        }

        // WiFi 수신 처리
        struct timeval timeout = {0, 100000};
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        if (select(server_fd + 1, &readfds, NULL, NULL, &timeout) > 0 &&
            FD_ISSET(server_fd, &readfds)) {

            client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
            if (client_fd < 0) {
                perror("accept 실패");
                goto cleanup;
            }

            int net_result;
            int recv_len = recv(client_fd, &net_result, sizeof(net_result), 0);
            if (recv_len == sizeof(net_result)) {
                int received_yut_result = ntohl(net_result);

                if (attempt > 0) {
                    if (received_yut_result == 1 || received_yut_result == 2 ||
                        received_yut_result == 3 || received_yut_result == 4 ||
                        received_yut_result == 5 || received_yut_result == 8) {

                        insert_fnd_number(received_yut_result);

                        MyMsgBuf msg;
                        msg.mtype = MSG_TYPE_YUT;
                        msg.yut_result = received_yut_result;

                        if (msgsnd(msgid, &msg, sizeof(MyMsgBuf) - sizeof(long), 0) == -1) {
                            perror("msgsnd 실패");
                        } else {
                            printf("▶ Qt로 IPC 메시지 전송됨: %d\n", received_yut_result);
                        }

                        if (received_yut_result == 4 || received_yut_result == 5) {
                            decrement_attempt();
                            increment_attempt(); // 한번 더 기회
                        } else {
                            decrement_attempt();
                        }
                    } else {
                        fprintf(stderr, "▶ 유효하지 않은 윷 결과 수신: %d\n", received_yut_result);
                    }
                } else {
                    printf("▶ attempt = 0 상태. 수신값 무시됨.\n");
                }
            } else {
                perror("recv 실패 또는 연결 종료");
            }

            close(client_fd);
            client_fd = -1;
        }

        check_and_toggle_player();
        usleep(10000);
    }

cleanup:
    printf("▶ 시스템 종료 중...\n");
    if (client_fd != -1) close(client_fd);
    if (server_fd != -1) close(server_fd);
    if (button_fd != -1) close(button_fd);
    return 1;
}
