#ifndef YUTBOARD_H
#define YUTBOARD_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QWidget>
#include <QPoint>
#include <QRect>
#include <QTimer>

extern "C" {
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ipc_messages.h"
}


class YutGame;

class yutboard : public QWidget
{
    Q_OBJECT

public:
    explicit yutboard(QWidget *parent = nullptr);
    void setGame(YutGame *game);
    void selectPiece(int index);       // 0 또는 1 (플레이어의 첫 번째/두 번째 말 선택)
    void moveSelectedPiece();          // 선택된 말 이동


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
//    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void checkIpcQueue();

private:
    QPoint getScreenPos(int locId);
    YutGame *m_game;
    QPoint boardPoints[31]; // 충분히 크게 잡아둠
    bool m_started = false;
    QRect m_startButton;

    // IPC 멤버
    int     m_msgQueueId    = -1;    // 메시지 큐 식별자
    QTimer* m_ipcTimer      = nullptr;
};

#endif // YUTBOARD_H
