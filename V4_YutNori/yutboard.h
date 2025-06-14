#ifndef YUTBOARD_H
#define YUTBOARD_H

#include <QWidget>
#include <QPoint>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QKeyEvent>

class YutGame;  // 기존


class yutboard : public QWidget
{
    Q_OBJECT

public:
    explicit yutboard(QWidget *parent = nullptr);
    void setGame(YutGame *game);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    QPoint getScreenPos(int locId);

    YutGame *m_game;
    QPoint boardPoints[31];

    // ↓ 추가
    bool    m_started    = false;   // 게임 시작 플래그
    QRect   m_startButton;          // Start 버튼 위치
};

#endif // YUTBOARD_H
