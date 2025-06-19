#include <stdio.h>
#include <unistd.h>
#include "Yut_logic.h"
#include "wifi_sender.h"

int main(void) {
    while (1) {
        int result = get_yut_result();  // 정수형 결과
        if (result != -1) {
            printf("윷 결과 (1=도, 2=개, 3=걸, 4=윷, 5=모, 8=빽도) : %d\n", result);
            send_result_over_wifi(result);  // 문자열 변환 없이 전송
            usleep(500000);  // 0.5초 대기
        }
        usleep(100000);  // 반복 주기
    }

    return 0;
}
