#include "yutboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QTimer>
#include <QImage>
#include <QKeyEvent>
#include <QDebug>

#include "defvar.h"
#include "initialize.h"
#include "toss.h"
#include "select.h"
#include "move.h"
#include "movemal.h"
#include "moveresult.h"
#include "rollyut.h"

yutboard::yutboard(QWidget* parent)
    : QWidget(parent)
{
    // 난수 시드 초기화 (윷 던질 때 rand() 사용 대비)
    initToss();

    // 게임 로직 초기화
    initialize();

    // 마우스 클릭·키 입력 받도록 포커스 설정
    setFocusPolicy(Qt::StrongFocus);

    // 첫 번째 던지기 준비
    ready = ON;

    // 보드 위 좌표 설정
    boardPoints[0]  = QPoint(600, 600);
    boardPoints[1]  = QPoint(653, 274);
    boardPoints[2]  = QPoint(653, 210);
    boardPoints[3]  = QPoint(653, 146);
    boardPoints[4]  = QPoint(653, 82);
    boardPoints[5]  = QPoint(653, 28);
    boardPoints[6]  = QPoint(536, 28);
    boardPoints[7]  = QPoint(417, 28);
    boardPoints[8]  = QPoint(289, 28);
    boardPoints[9]  = QPoint(166, 28);
    boardPoints[10] = QPoint(60, 28);
    boardPoints[11] = QPoint(60, 82);
    boardPoints[12] = QPoint(60, 146);
    boardPoints[13] = QPoint(60, 210);
    boardPoints[14] = QPoint(60, 274);
    boardPoints[15] = QPoint(60, 326);
    boardPoints[16] = QPoint(166, 326);
    boardPoints[17] = QPoint(289, 326);
    boardPoints[18] = QPoint(417, 326);
    boardPoints[19] = QPoint(536, 326);
    boardPoints[20] = QPoint(653, 326);
    boardPoints[21] = QPoint(536, 85);
    boardPoints[22] = QPoint(445, 131);
    boardPoints[23] = QPoint(357, 176);
    boardPoints[24] = QPoint(262, 222);
    boardPoints[25] = QPoint(172, 275);
    boardPoints[26] = QPoint(162, 85);
    boardPoints[27] = QPoint(262, 131);
    boardPoints[28] = QPoint(445, 223);
    boardPoints[29] = QPoint(536, 274);
    boardPoints[30] = QPoint(600, 600);

    // 이동/화면 갱신 타이머 (0.1초)
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        if (playMode == MOVE) {
            moveStep();
        }
        update();
    });
    timer->start(100);
}

void yutboard::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    qDebug() << "Board clicked!";

    if (playMode == TOSS) {
        // 마우스 클릭으로 윷 던지기
        ready = ON;
        qDebug() << "→ toss() 호출";
        toss();      // → playMode = SELECT
    }
    else if (playMode == SELECT) {
        select();    // 말 선택 → playMode = MOVE
    }
}

void yutboard::keyPressEvent(QKeyEvent* ev)
{
    if ((ev->key() == Qt::Key_Space || ev->key() == Qt::Key_Return)
        && playMode == TOSS)
    {
        qDebug() << "→ toss() 호출";
        toss();    // 이제 이 한 줄로 윷 던지기가 실행됩니다
    }
    if (playMode == SELECT) {
        int k = -1;
        if      (ev->key() == Qt::Key_1) k = 0;
        else if (ev->key() == Qt::Key_2) k = 1;
        if (k >= 0) {
            // curP==P1 이면 P11,P12, 아니라면 P21,P22
            malSelected = (curP == P1 ? P11 : P21) + k;
            qDebug() << "Player" << (curP==P1?1:2)
                     << "말" << (k+1) << "선택됨";
            // 선택 로직 실행 (콘솔용 select() 대신 바로 MOVE 모드 진입)
            playMode = MOVE;
            return;
        }
    }
}

void yutboard::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // 배경보드 그리기 (리소스나 절대경로로 설정)
    static QImage boardImage("/home/yoo/YutNori/board.jpg");
    painter.drawImage(rect(), boardImage);

    // 모든 말 그리기
    for (int m = 0; m < numMal; ++m) {
        QPoint pt = boardPoints[ loc[m] ];
        QColor c = (m < 4 ? Qt::blue : Qt::red);
        painter.setBrush(c);
        painter.setPen(Qt::black);
        painter.drawEllipse(pt, 10, 10);
    }
}
