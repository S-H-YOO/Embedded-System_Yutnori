CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar

TARGET = Yut_throw
LIBNAME = libAccelMagGyro.a

all: $(TARGET)

$(TARGET): $(LIBNAME) Yut_throw.c
	$(CC) -o $(TARGET) Yut_throw.c -lAccelMagGyro -L.

$(LIBNAME): AccelMagGyro.o
	$(AR) rc $(LIBNAME) AccelMagGyro.o

AccelMagGyro.o: AccelMagGyro.c AccelMagGyro.h
	$(CC) -c AccelMagGyro.c -o AccelMagGyro.o

clean:
	rm -f $(TARGET) AccelMagGyro.o $(LIBNAME)
