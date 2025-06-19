#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "AccelMagGyro.h"
#include "Yut_logic.h"

int get_yut_result(void) {
    int accel1, accel2, accel3;
    int accel_avg;
    int max_avg;

    Accel(&accel1, &accel2, &accel3);
    accel_avg = (abs(accel1) + abs(accel2) + abs(accel3)) / 300;

    if (accel_avg < 100)
        return -1;

    max_avg = accel_avg;
    for (int i = 0; i < 10; i++) {
        Accel(&accel1, &accel2, &accel3);
        accel_avg = (abs(accel1) + abs(accel2) + abs(accel3)) / 300;
        if (accel_avg > max_avg)
            max_avg = accel_avg;
        usleep(100000);
    }

    srand(max_avg);
    int r = rand() % 1000;

    if (r < 53) return 8;
    else if (r < 211) return 1;
    else if (r < 581) return 2;
    else if (r < 871) return 3;
    else if (r < 956) return 4;
    else return 5;
}
