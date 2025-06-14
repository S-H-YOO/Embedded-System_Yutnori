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
    QPoint pos = event->pos();
    qDebug() << "Clicked at:" << pos.x() << "," << pos.y();
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

    // 1) 배경 보드
    static QImage boardImage("/home/yoo/YutNori/board.jpg");
    painter.drawImage(rect(), boardImage);

    // 2) 윷 결과 아이콘
    QString imgPath;
    switch (result) {
    case -1: imgPath = "/home/yoo/YutNori//빽도.png"; break;
    case 1:     imgPath = "/home/yoo/YutNori//도.png";     break;
    case 2:    imgPath = "/home/yoo/YutNori//개.png";    break;
    case 3:   imgPath = "/home/yoo/YutNori//걸.png";   break;
    case 4:    imgPath = "/home/yoo/YutNori//윷.png";    break;
    case 5:     imgPath = "/home/yoo/YutNori//모.png";     break;
    default:     imgPath.clear();              break;
    }
    if (!imgPath.isEmpty()) {
        QImage icon(imgPath);
        // 원하는 위치/크기로 조정하세요.
        painter.drawImage(QRect(260, 340, 200, 100), icon);
    }

    // 3) 폰트 준비 (업힘 숫자)
    QFont nfont = painter.font();
    nfont.setBold(true);
    nfont.setPointSize(12);
    painter.setFont(nfont);

    // 4) station[] 기반으로 말 그리기 & 업힘 숫자
    for (int idx = 0; idx < 31; ++idx) {
        int st = station[idx];
        if (st == EMPTY) continue;

        // (a) 색 결정: 1P 관련 상태면 파랑, 2P 관련 상태면 빨강
        QColor fillColor;
        if ((st >= P11 && st <= P14) || (st >= DOUBLE1 && st <= QUAD1)) {
            fillColor = Qt::blue;
        } else if ((st >= P21 && st <= P24) || (st >= DOUBLE2 && st <= QUAD2)) {
            fillColor = Qt::red;
        } else {
            continue;
        }

        // (b) 원 그리기
        QPoint pt = boardPoints[idx];
        painter.setBrush(fillColor);
        painter.setPen(Qt::black);
        painter.drawEllipse(pt, 10, 10);

        // (c) 업힘 숫자 라벨
        QString label;
        switch (st) {
        case DOUBLE1: case DOUBLE2: label = "2"; break;
        case TRIPLE1: case TRIPLE2: label = "3"; break;
        case QUAD1:   case QUAD2:   label = "4"; break;
        default: break;
        }
        if (!label.isEmpty()) {
            painter.setPen(Qt::white);
            painter.drawText(
                QRect(pt - QPoint(10,10), QSize(20,20)),
                Qt::AlignCenter,
                label
                );
        }
    }

}
