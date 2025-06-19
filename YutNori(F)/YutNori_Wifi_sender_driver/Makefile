CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar
CFLAGS = -Wall

LIB_SRCS = AccelMagGyro.c Yut_logic.c wifi_sender.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
LIB_NAME = libYutLib.a

TARGET = Yut_wifi
MAIN_SRC = main.c
MAIN_OBJ = $(MAIN_SRC:.c=.o)

all: $(LIB_NAME) $(TARGET)

# 라이브러리 생성
$(LIB_NAME): $(LIB_OBJS)
	$(AR) rc $@ $^

# 실행파일 생성 (라이브러리 링크)
$(TARGET): $(MAIN_OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $(MAIN_OBJ) -lYutLib -L.

# 개별 오브젝트 파일 컴파일 규칙
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_OBJS) $(MAIN_OBJ) $(LIB_NAME) $(TARGET)
