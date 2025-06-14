#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "AccelMagGyro.h"

int main(void){

    int accel1, accel2, accel3;
    int accel_avg;
    int max_avg;

    while(1)
    {
        Accel(&accel1, &accel2, &accel3);
        accel_avg = (abs(accel1) + abs(accel2) + abs(accel3)) / 300;

        if (accel_avg >= 100)
        {
            max_avg = accel_avg;

            // 1초 동안 측정하여 max_avg 갱신
            for (int i = 0; i < 10; i++)
            {
                Accel(&accel1, &accel2, &accel3);
                accel_avg = (abs(accel1) + abs(accel2) + abs(accel3)) / 300;
                if (accel_avg > max_avg)
                    max_avg = accel_avg;
                usleep(100000);
            }

            // max_avg를 시드로 사용
            srand(max_avg);
            int r = rand() % 1000;

            const char* result;
            if (r < 53)
                result = "빽도";
            else if (r < 211)
                result = "도";
            else if (r < 581)
                result = "개";
            else if (r < 871)
                result = "걸";
            else if (r < 956)
                result = "윷";
            else
                result = "모";

            printf("▶ 윷 결과: %s\n", result);

            usleep(500000);  // 중복 방지 대기
        }

        usleep(100000);
    }

    return 0;
}
