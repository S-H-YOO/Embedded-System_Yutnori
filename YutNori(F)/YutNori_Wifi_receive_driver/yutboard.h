#ifndef YUTBOARD_H
#define YUTBOARD_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QTimer>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "defvar.h"

extern unsigned char blink_state;
extern unsigned char mal_to_blink;

class yutboard : public QWidget
{
    Q_OBJECT

public:
    explicit yutboard(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateBoard();

private:
    QPoint boardPoints[50]; // 충분히 크게 잡아둠
    QTimer* updateTimer;
    int msg_queue_id;
};

#endif // YUTBOARD_H
