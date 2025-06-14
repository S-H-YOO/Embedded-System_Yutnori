# 크로스 컴파일러 설정
CC = arm-linux-gnueabi-gcc
AR = arm-linux-gnueabi-ar
CFLAGS = -Wall -g

# 실행 타겟 이름
TARGET = Yut_game_exec

# 정적 라이브러리 이름 및 오브젝트 목록
STATIC_LIB = libyutlib.a
LIB_OBJS = fnd_control.o button_fnd.o wifi_receiver.o textlcd.o

# 메인 오브젝트 파일
MAIN_OBJ = main.o

INCLUDES = -I.

# 기본 타겟
all: $(TARGET)

# 실행 파일 생성
$(TARGET): $(MAIN_OBJ) $(STATIC_LIB)
	$(CC) -o $@ $(MAIN_OBJ) -L. -lyutlib

# 정적 라이브러리 생성
$(STATIC_LIB): $(LIB_OBJS)
	$(AR) rcs $@ $^

# 개별 소스 → 오브젝트 컴파일
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $< $(INCLUDES)

# 정리
clean:
	rm -f $(TARGET) $(STATIC_LIB) *.o
