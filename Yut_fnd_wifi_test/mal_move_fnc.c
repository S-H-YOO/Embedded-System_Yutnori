#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>

#include "fnddrv.h"

#define SERVER_PORT 9999
#define BUFFER_SIZE 1024
#define FND_DEVICE "/dev/perifnd"

// 현재 FND에 출력할 숫자 배열 (최대 6자리, -1은 공백)
static int fnd_digits[6] = {-1, -1, -1, -1, -1, -1};

// 문자열을 숫자로 변환하는 함수
static int yut_result_to_number(const char* result) {
    if (strcmp(result, "빽도") == 0) return 8;
    else if (strcmp(result, "도") == 0) return 1;
    else if (strcmp(result, "개") == 0) return 2;
    else if (strcmp(result, "걸") == 0) return 3;
    else if (strcmp(result, "윷") == 0) return 4;
    else if (strcmp(result, "모") == 0) return 5;
    else return -1;  // 유효하지 않은 입력
}

// 결과를 FND에 출력하는 함수
int display_yut_result(const char* result) {
    int number = yut_result_to_number(result);
    if (number == -1) {
        fprintf(stderr, "잘못된 윷 결과: %s\n", result);
        return -1;
    }

    // 배열을 왼쪽으로 한 칸 밀고 마지막에 새 숫자 추가
    for (int i = 0; i < 5; i++) {
        fnd_digits[i] = fnd_digits[i + 1];
    }
    fnd_digits[5] = number;

    // FND 디바이스 열기
    int dev = open(FND_DEVICE, O_WRONLY);
    if (dev < 0) {
        perror("FND 디바이스 열기 실패");
        return -1;
    }

    // 출력 형식 설정
    stFndWriteForm fnd_data;
    memset(&fnd_data, 0, sizeof(fnd_data));

    for (int i = 0; i < 6; i++) {
        if (fnd_digits[i] >= 0) {
            fnd_data.DataValid[i] = 1;
            fnd_data.DataNumeric[i] = fnd_digits[i];
            fnd_data.DataDot[i] = 0;
        }
    }

    // FND 출력
    if (write(dev, &fnd_data, sizeof(fnd_data)) < 0) {
        perror("FND 쓰기 실패");
        close(dev);
        return -1;
    }

    close(dev);
    return 0;
}

// 메인 함수: WiFi로 윷 결과 수신
int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t client_len = sizeof(client_addr);

    // 소켓 생성
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // 바인드
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // 리슨
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("▶ 윷 결과 수신 대기 중... 포트 %d\n", SERVER_PORT);

    while (1) {
        // 연결 수락
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        // 데이터 수신
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            printf("▶ 수신된 윷 결과: %s\n", buffer);

            // FND 출력
            if (display_yut_result(buffer) == 0) {
                printf("▶ FND 출력 완료: %s\n", buffer);
            } else {
                fprintf(stderr, "▶ FND 출력 실패: %s\n", buffer);
            }
        }

        // 클라이언트 연결 종료
        close(client_fd);
    }

    close(server_fd);
    return 0;
}
