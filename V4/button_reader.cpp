#include <fcntl.h>
#include <unistd.h>
#include <QDebug>
#include <QMetaObject>
#include "mainwindow.h"
#include "button_reader.h"


void* readButtonFIFO(void* arg) {
    MainWindow* w = static_cast<MainWindow*>(arg);
    int fd = open("/tmp/button_fifo", O_RDONLY);
    if (fd < 0) {
        perror("open fifo");
        return nullptr;
    }

    char input;
    while (1) {
        int r = read(fd, &input, 1);
        if (r > 0) {
            qDebug() << "[FIFO 읽음] input =" << input;
            QMetaObject::invokeMethod(w, "handleButtonInput", Qt::QueuedConnection, Q_ARG(char, input));
        }
    }

    close(fd);
    return nullptr;
}

