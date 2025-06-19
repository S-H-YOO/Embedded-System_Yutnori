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

// IPC 관련 헤더 추가
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ipc_messages.h"

int msgid;  // 메시지 큐 ID

yutboard::yutboard(QWidget* parent)
    : QWidget(parent)
{
    initToss();
    initialize();
    setMinimumSize(711,501);
    setFocusPolicy(Qt::StrongFocus);
    ready = ON;

    key_t key = ftok(MSG_KEY_PATH, MSG_KEY_ID);
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget 실패");
    }

    boardPoints[0]  = QPoint(800, 800);
    boardPoints[1]  = QPoint(653, 390);
    boardPoints[2]  = QPoint(653, 300);
    boardPoints[3]  = QPoint(653, 210);
    boardPoints[4]  = QPoint(653, 120);
    boardPoints[5]  = QPoint(653, 36);
    boardPoints[6]  = QPoint(536, 36);
    boardPoints[7]  = QPoint(411, 36);
    boardPoints[8]  = QPoint(289, 36);
    boardPoints[9]  = QPoint(163, 36);
    boardPoints[10] = QPoint(60, 36);
    boardPoints[11] = QPoint(58, 120);
    boardPoints[12] = QPoint(58, 210);
    boardPoints[13] = QPoint(58, 300);
    boardPoints[14] = QPoint(58, 390);
    boardPoints[15] = QPoint(58, 465);
    boardPoints[16] = QPoint(171, 465);
    boardPoints[17] = QPoint(292, 465);
    boardPoints[18] = QPoint(418, 465);
    boardPoints[19] = QPoint(541, 465);
    boardPoints[20] = QPoint(653, 465);
    boardPoints[21] = QPoint(536, 118);
    boardPoints[22] = QPoint(445, 183);
    boardPoints[23] = QPoint(355, 249);
    boardPoints[24] = QPoint(262, 315);
    boardPoints[25] = QPoint(172, 390);
    boardPoints[26] = QPoint(162, 117);
    boardPoints[27] = QPoint(262, 184);
    boardPoints[28] = QPoint(445, 316);
    boardPoints[29] = QPoint(542, 388);
    boardPoints[30] = QPoint(800, 800);

    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=]() {
        checkMessageQueue();  // 메시지 수신
        if (playMode == MOVE) {
            moveStep();
        }
        update();
    });
    timer->start(100);
}

void yutboard::checkMessageQueue() {
    // 윷 결과 수신
    MyMsgBuf msg;
    ssize_t r = msgrcv(msgid, &msg, sizeof(msg.yut_result), MSG_TYPE_YUT, IPC_NOWAIT);
    if (r > 0) {
        qDebug() << "메시지 수신됨: 윷 결과 =" << msg.yut_result;
        if (msg.yut_result == 8)
            result = -1;
        else if (msg.yut_result >= 1 && msg.yut_result <= 5)
            result = msg.yut_result;
    }

    // 말 이동 수신
    MoveMalMsgBuf move_msg;
    ssize_t r2 = msgrcv(msgid, &move_msg, sizeof(move_msg) - sizeof(long), MSG_TYPE_MOVE_MAL, IPC_NOWAIT);
    if (r2 > 0) {
        qDebug() << "말 이동 메시지 수신: 말 인덱스 =" << move_msg.mal_id << ", 이동칸 =" << move_msg.move_steps;

        int base = (curP == P1) ? P11 : P21;
        int target = base + move_msg.mal_id;

        if (status[target] != SCORE) {
            malSelected = target;
            result = move_msg.move_steps;
            playMode = MOVE;
        }
    }
}

void yutboard::mousePressEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();
    if (!m_started) {
        if (m_startButton.contains(pos)) {
            m_started = true;
            update();
        }
        return;
    }

    if (playMode == TOSS) {
        ready = ON;
        toss();
    }
}

void yutboard::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!m_started) {
        painter.fillRect(rect(), Qt::white);
        QFont font = painter.font();
        font.setPointSize(24);
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, "Touch to Start");

        int w = 200, h = 80;
        int x = (width() - w) / 2;
        int y = (height() - h) / 2 + 100;
        m_startButton = QRect(x, y, w, h);

        painter.setBrush(Qt::gray);
        painter.drawRoundedRect(m_startButton, 10, 10);
        painter.setPen(Qt::white);
        font.setPointSize(18);
        painter.setFont(font);
        painter.drawText(m_startButton, Qt::AlignCenter, "Start");

        return;
    }

    static QImage boardImage("./board.jpg");
    painter.drawImage(rect(), boardImage);

    QString imgPath;
    switch (result) {
    case -1: imgPath = "./빽도.png"; break;
    case 1:  imgPath = "./도.png"; break;
    case 2:  imgPath = "./개.png"; break;
    case 3:  imgPath = "./걸.png"; break;
    case 4:  imgPath = "./윷.png"; break;
    case 5:  imgPath = "./모.png"; break;
    default: imgPath.clear(); break;
    }

    if (!imgPath.isEmpty()) {
        QImage icon(imgPath);
        painter.drawImage(QRect(260, 340, 200, 100), icon);
    }

    QFont nfont = painter.font();
    nfont.setBold(true);
    nfont.setPointSize(12);
    painter.setFont(nfont);

    for (int idx = 0; idx < 30; ++idx) {
        int st = station[idx];
        if (st == EMPTY) continue;

        QColor fillColor;
        if ((st >= P11 && st <= P14) || (st >= DOUBLE1 && st <= QUAD1)) {
            fillColor = Qt::blue;
        } else if ((st >= P21 && st <= P24) || (st >= DOUBLE2 && st <= QUAD2)) {
            fillColor = Qt::red;
        } else {
            continue;
        }

        QPoint pt = boardPoints[idx];
        painter.setBrush(fillColor);
        painter.setPen(Qt::black);
        painter.drawEllipse(pt, 10, 10);

        QString label;
        switch (st) {
        case DOUBLE1: case DOUBLE2: label = "2"; break;
        case TRIPLE1: case TRIPLE2: label = "3"; break;
        case QUAD1:   case QUAD2:   label = "4"; break;
        default: break;
        }
        if (!label.isEmpty()) {
            painter.setPen(Qt::white);
            painter.drawText(QRect(pt - QPoint(10,10), QSize(20,20)), Qt::AlignCenter, label);
        }
    }
}

void yutboard::selectPiece(int pieceIndex)
{
    if (playMode != SELECT) return;

    int candidate = (curP == P1 ? P11 : P21) + pieceIndex;
    if (status[candidate] == SCORE) return;

    malSelected = candidate;
    playMode = MOVE;
}

void yutboard::moveSelectedPiece()
{
    if (playMode != MOVE) return;
    moveMal(malSelected);
}

