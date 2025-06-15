// button_ipc.cpp
#include "input_keymap.h"
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>
#include <sys/stat.h>
#include <iostream>

void* buttonListenerThread(void* arg) {
    mkfifo("/tmp/button_fifo", 0666);

    int fd_btn = open("/dev/input/event7", O_RDONLY);
    int fd_fifo = open("/tmp/button_fifo", O_WRONLY);
    if (fd_btn < 0 || fd_fifo < 0) {
        perror("open");
        return nullptr;
    }

    struct input_event ev;
    while (1) {
        ssize_t n = read(fd_btn, &ev, sizeof(ev));
        if (n == sizeof(ev)) {
            if (ev.type == EV_KEY && ev.value == 1) {  // 눌림 이벤트
                auto it = keycodeToChar.find(ev.code);
                if (it != keycodeToChar.end()) {
                    char mapped = it->second;
                    write(fd_fifo, &mapped, 1);
                }
            }
        }
    }

    close(fd_btn);
    close(fd_fifo);
    return nullptr;
}

void startButtonListener() {
    pthread_t tid;
    pthread_create(&tid, nullptr, buttonListenerThread, nullptr);
    pthread_detach(tid);
}

