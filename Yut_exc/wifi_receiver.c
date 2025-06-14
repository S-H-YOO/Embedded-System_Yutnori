#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "fnd_control.h"

#define SERVER_PORT 9999

const char* number_to_yut_result(int num) {
    switch (num) {
        case 8: return "빽도";
        case 1: return "도";
        case 2: return "개";
        case 3: return "걸";
        case 4: return "윷";
        case 5: return "모";
        default: return "유효하지 않음";
    }
}

void run_wifi_server() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("▶ WiFi 정수 수신 대기 중...\n");

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        int net_result;
        int recv_len = recv(client_fd, &net_result, sizeof(net_result), 0);
        if (recv_len == sizeof(net_result)) {
            int result = ntohl(net_result);  // 네트워크 바이트 → 호스트 바이트
            printf("▶ 수신된 윷 결과 (int): %d (%s)\n", result, number_to_yut_result(result));

            if (result == 1 || result == 2 || result == 3 || result == 4 || result == 5 || result == 8) {
                insert_fnd_number(result);
            } else {
                fprintf(stderr, "▶ 유효하지 않은 윷 값: %d\n", result);
            }
        } else {
            fprintf(stderr, "▶ 수신 실패 또는 잘못된 크기: %d\n", recv_len);
        }

        close(client_fd);
    }

    close(server_fd);
}
