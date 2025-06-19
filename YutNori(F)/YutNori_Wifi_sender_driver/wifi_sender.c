#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "wifi_sender.h"

#define SERVER_IP "192.168.224.54"
#define SERVER_PORT 9999

void send_result_over_wifi(int result) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return;
    }

    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(SERVER_PORT),
    };
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return;
    }

    int net_result = htonl(result);  // 네트워크 바이트 순서로 전환
    send(sock, &net_result, sizeof(net_result), 0);  // 정수 그대로 전송

    close(sock);
}
