#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "fnd_control.h"
#include "button_fnd.h"
#include "wifi_receiver.h"
#include "textlcd.h"

#define BUTTON_DEVICE "/dev/input/event5"
#define SERVER_PORT 9999

extern int attempt;

int main() {
    int button_fd, server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // FND 및 LCD 초기화
    init_fnd_digits();
    update_fnd_display();
    update_textlcd_display();

    // 버튼 디바이스 열기
    button_fd = open(BUTTON_DEVICE, O_RDONLY | O_NONBLOCK);
    if (button_fd < 0) {
        perror("버튼 디바이스 열기 실패");
        return 1;
    }

    // WiFi 수신 소켓 초기화
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket 생성 실패");
        close(button_fd);
        return 1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind 실패");
        close(server_fd);
        close(button_fd);
        return 1;
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen 실패");
        close(server_fd);
        close(button_fd);
        return 1;
    }

    printf("▶ 시스템 실행 중...\n");

    while (1) {
        // 버튼 이벤트 처리
        struct input_event ev;
        if (read(button_fd, &ev, sizeof(ev)) == sizeof(ev)) {
            if (ev.type == EV_KEY && ev.value == 1) {
                if (ev.code == BUTTON_CODE_MOVE) {
                    move_cursor_left();
                } else if (ev.code == BUTTON_CODE_DELETE) {
                    delete_and_shift();
                } else if (ev.code == KEY_VOLUMEDOWN) {  // 시도 횟수 증가
                    increment_attempt();
                }
            }
        }

        // WiFi 수신 처리 (논블로킹)
        struct timeval timeout = {0, 100000};  // 100ms
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        int sel = select(server_fd + 1, &readfds, NULL, NULL, &timeout);
        if (sel > 0 && FD_ISSET(server_fd, &readfds)) {
            client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
            if (client_fd < 0) {
                perror("accept 실패");
                continue;
            }

            int net_result;
            int recv_len = recv(client_fd, &net_result, sizeof(net_result), 0);
            if (recv_len == sizeof(net_result)) {
                int result = ntohl(net_result);

                if (attempt > 0) {
                    if (result == 1 || result == 2 || result == 3 || result == 4 || result == 5 || result == 8) {
                        insert_fnd_number(result);

                        if (result == 4 || result == 5)
                            increment_attempt();  // 윷 또는 모 → 시도 증가
                        else
                            decrement_attempt();  // 일반 → 시도 감소
                    }
                } else {
                    printf("▶ attempt 0: 수신된 값 무시됨 (%d)\n", result);
                }
            } else {
                fprintf(stderr, "▶ 수신 오류\n");
            }
            close(client_fd);
        }

        // FND 비어 있고 attempt == 0이면 player 전환
        check_and_toggle_player();

        usleep(10000);  // CPU 점유율 방지
    }

    close(server_fd);
    close(button_fd);
    return 0;
}
