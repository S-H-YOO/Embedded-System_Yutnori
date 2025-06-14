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
    if (!m_started) {
        // 시작 화면에서 Start 버튼 클릭 시
        if (m_startButton.contains(event->pos())) {
            m_started = true;
            update();  // 화면 갱신해서 게임 화면으로 전환
        }
        return;
    }

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

    if (!m_started) {
        // **시작 화면**
        painter.fillRect(rect(), Qt::white);
        QFont f = painter.font();
        f.setPointSize(24);
        painter.setFont(f);
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, "YutNori\n\nTouch to Start");

        // Start 버튼 영역 계산 (가운데에 200×80 크기로)
        int w = 200, h = 80;
        int x = (width() - w) / 2;
        int y = (height() - h) / 2 + 100;
        m_startButton = QRect(x, y, w, h);

        // 버튼 그리기
        painter.setBrush(Qt::gray);
        painter.drawRoundedRect(m_startButton, 10, 10);
        painter.setPen(Qt::white);
        f.setPointSize(18);
        painter.setFont(f);
        painter.drawText(m_startButton, Qt::AlignCenter, "Start");

        return;
    }

    // **게임 화면** (기존 코드 그대로)
    static QImage boardImage("./board.jpg");
    painter.drawImage(rect(), boardImage);

    QString imgPath;
    switch (result) {
    case -1: imgPath = "./빽도.png"; break;
    case 1:     imgPath = "./도.png";     break;
    case 2:    imgPath = "./개.png";    break;
    case 3:   imgPath = "./걸.png";   break;
    case 4:    imgPath = "./윷.png";    break;
    case 5:     imgPath = "./모.png";     break;
    default:     imgPath.clear();              break;
    }

    // 2) 그림이 있으면 화면에 그리기 (좌상단 10×10 위치에 64×64 크기로)
    if (!imgPath.isEmpty()) {
        QImage icon(imgPath);
        painter.drawImage(QRect(275,240,150,70), icon);
    }

    // 모든 말 그리기
    for (int m = 0; m < numMal; ++m) {
        QPoint pt = boardPoints[ loc[m] ];
        QColor c = (m < 4 ? Qt::blue : Qt::red);
        painter.setBrush(c);
        painter.setPen(Qt::black);
        painter.drawEllipse(pt, 10, 10);
    }
}
