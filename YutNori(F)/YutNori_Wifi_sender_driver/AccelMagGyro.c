#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define ACCELPATH "/sys/class/misc/FreescaleAccelerometer/"
#define MAGNEPATH "/sys/class/misc/FreescaleMagnetometer/"
#define GYROPATH "/sys/class/misc/FreescaleGyroscope/"

int Accel(int*accel1,int*accel2,int*accel3)
{
//Accelerometer
int fd = 0;
FILE *fp = NULL;
fd = open (ACCELPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (ACCELPATH "data", "rt");
fscanf(fp,"%d, %d, %d",accel1,accel2,accel3);
fclose(fp);
return(0);
}

int Mage(int *magne1, int *magne2, int *magne3) {
//Magnetometer
int fd = 0;
FILE *fp = NULL;
fd = open (MAGNEPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (MAGNEPATH "data", "rt");
fscanf(fp,"%d, %d, %d",magne1, magne2, magne3);
fclose(fp);
return(0);
}

int Gyro(int *gyro1, int *gyro2, int *gyro3) {
//Gyroscope
int fd = 0;
FILE *fp = NULL;
fd = open (GYROPATH "enable",O_WRONLY);
dprintf (fd,"1");
close(fd);
fp = fopen (GYROPATH "data", "rt");
fscanf(fp,"%d, %d, %d",gyro1, gyro2, gyro3);
fclose(fp);
return(0);
}