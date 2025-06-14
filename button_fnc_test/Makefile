CC = arm-linux-gnueabi-gcc
CFLAGS = -Wall -O2
TARGET = fnd_btn_test

SRCS = fnd_btn_test.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
