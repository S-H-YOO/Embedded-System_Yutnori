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

#include <sys/ipc.h>
#include <sys/msg.h>
#include "ipc_messages.h"

yutboard::yutboard(QWidget* parent)
    : QWidget(parent)
{

    // 난수 시드 초기화 (윷 던질 때 rand() 사용 대비)
    initToss();

    // 게임 로직 초기화
    initialize();

    setMinimumSize(711,501);

    // 마우스 클릭·키 입력 받도록 포커스 설정
    setFocusPolicy(Qt::StrongFocus);

    // 첫 번째 던지기 준비
    ready = ON;


    key_t key = ftok(MSG_KEY_PATH, MSG_KEY_ID);
    m_msgQueueId = msgget(key, IPC_CREAT | 0666);
    if (m_msgQueueId < 0) {
        qWarning() << "IPC 메시지 큐 연결 실패";
    }
    else if (m_msgQueueId >= 0) {
        qWarning() << "IPC 메시지 큐 연결 실패";
    }
    m_ipcTimer = new QTimer(this);
    connect(m_ipcTimer, &QTimer::timeout, this, &yutboard::checkIpcQueue);
    m_ipcTimer->start(100);
    // 보드 위 좌표 설정
//    boardPoints[0]  = QPoint(600, 600);
//    boardPoints[1]  = QPoint(653, 274);
//    boardPoints[2]  = QPoint(653, 210);
//    boardPoints[3]  = QPoint(653, 146);
//    boardPoints[4]  = QPoint(653, 82);
//    boardPoints[5]  = QPoint(653, 28);
//    boardPoints[6]  = QPoint(536, 28);
//    boardPoints[7]  = QPoint(417, 28);
//    boardPoints[8]  = QPoint(289, 28);
//    boardPoints[9]  = QPoint(166, 28);
//    boardPoints[10] = QPoint(60, 28);
//    boardPoints[11] = QPoint(60, 82);
//    boardPoints[12] = QPoint(60, 146);
//    boardPoints[13] = QPoint(60, 210);
//    boardPoints[14] = QPoint(60, 274);
//    boardPoints[15] = QPoint(60, 326);
//    boardPoints[16] = QPoint(166, 326);
//    boardPoints[17] = QPoint(289, 326);
//    boardPoints[18] = QPoint(417, 326);
//    boardPoints[19] = QPoint(536, 326);
//    boardPoints[20] = QPoint(653, 326);
//    boardPoints[21] = QPoint(536, 85);
//    boardPoints[22] = QPoint(445, 131);
//    boardPoints[23] = QPoint(357, 176);
//    boardPoints[24] = QPoint(262, 222);
//    boardPoints[25] = QPoint(172, 275);
//    boardPoints[26] = QPoint(162, 85);
//    boardPoints[27] = QPoint(262, 131);
//    boardPoints[28] = QPoint(445, 223);
//    boardPoints[29] = QPoint(536, 274);
//    boardPoints[30] = QPoint(600, 600);

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
    //Q_UNUSED(event);
    //qDebug() << "Board clicked!";

    QPoint pos = event->pos();  // 클릭 위치 가져오기
    qDebug() << "Board clicked!";

    // 1. 게임 시작 전: 시작 버튼 클릭 처리
    if (!m_started) {
        if (m_startButton.contains(pos)) {
            m_started = true;
            qDebug() << "게임 시작됨!";
            update();  // 화면 갱신
        }
        return;
    }

    if (playMode == TOSS) {
        // 마우스 클릭으로 윷 던지기
        ready = ON;
        qDebug() << "→ toss() 호출";
        toss();      // → playMode = SELECT
    }


//    else if (playMode == SELECT) {
//        select();    // 말 선택 → playMode = MOVE
//    }
}

void yutboard::checkIpcQueue()
{
    qDebug() << "[DEBUG] checkIpcQueue() 호출됨";
    if (m_msgQueueId < 0) return;
    MyMsgBuf msg;
    ssize_t r = msgrcv(m_msgQueueId, &msg, sizeof(msg.yut_result), MSG_TYPE_YUT, IPC_NOWAIT);
    if (r == sizeof(msg.yut_result)) {
        // IPC로 수신된 윷 결과
        result = msg.yut_result;
        // 다음 단계 진입
        playMode = SELECT;
        update();
        qDebug() << "IPC로 받은 윷 결과:" << result;
    }
}

//void yutboard::keyPressEvent(QKeyEvent* ev)
//{
//    if ((ev->key() == Qt::Key_Space || ev->key() == Qt::Key_Return)
//        && playMode == TOSS)
//    {
//        qDebug() << "→ toss() 호출";
//        toss();    // 이제 이 한 줄로 윷 던지기가 실행됩니다
//    }
//    if (playMode == SELECT) {
//        int k = -1;
//        if      (ev->key() == Qt::Key_1) k = 0;
//        else if (ev->key() == Qt::Key_2) k = 1;
//        if (k >= 0) {
//           
//	   ///////////////////////////////////////////////////////// reserved
//	   // // curP==P1 이면 P11,P12, 아니라면 P21,P22
//           // malSelected = (curP == P1 ? P11 : P21) + k;
//           // qDebug() << "Player" << (curP==P1?1:2)
//           //          << "말" << (k+1) << "선택됨";
//           // // 선택 로직 실행 (콘솔용 select() 대신 바로 MOVE 모드 진입)
//           // playMode = MOVE;
//           // return;
//
//            int candidate = (curP == P1 ? P11 : P21) + k;
//
//	    if (status[candidate] == SCORE) {
//	        qDebug() << "말이 이미 도착했습니다. 다시 선택하세요.";
//		return;
//	    }
//
//	    malSelected = candidate;
//	    qDebug() << "Player" << (curP==P1?1:2)
//		     << "말" << (k+1) << "선택됨";
//
//	    playMode = MOVE;
//	    return;
//
//        }
//    }
//}

void yutboard::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!m_started) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // 배경 + 텍스트
        painter.fillRect(rect(), Qt::white);
        QFont font = painter.font();
        font.setPointSize(24);
        painter.setFont(font);
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, "Touch to Start");

        // 시작 버튼
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

        return;  // 시작 전엔 말 안 그림
    }

    // 배경보드 그리기 (리소스나 절대경로로 설정)
    static QImage boardImage("./board.jpg");
    painter.drawImage(rect(), boardImage);

    QString imgPath;
    switch (result) {
    case 8: imgPath = "./빽도.png"; break;
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
        painter.drawImage(QRect(260, 340, 200, 100), icon);
        //painter.drawImage(QRect(300,150,250,150), icon);
    }

//    // 모든 말 그리기
//    for (int m = 0; m < numMal; ++m) {
//        QPoint pt = boardPoints[ loc[m] ];
//        QColor c = (m < 4 ? Qt::blue : Qt::red);
//        painter.setBrush(c);
//        painter.setPen(Qt::black);
//        painter.drawEllipse(pt, 10, 10);
//    }

// 3) 폰트 준비 (업힘 숫자)
    QFont nfont = painter.font();
    nfont.setBold(true);
    nfont.setPointSize(12);
    painter.setFont(nfont);

    // 4) station[] 기반으로 말 그리기 & 업힘 숫자
    for (int idx = 0; idx < 30; ++idx) {
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

// IPC에서도 재사용할 수 있도록 분리된 말 선택 함수
void yutboard::selectPiece(int pieceIndex)
{
    if (playMode != SELECT) return;

    int candidate = (curP == P1 ? P11 : P21) + pieceIndex;
    if (status[candidate] == SCORE) {
        qDebug() << "말이 이미 도착했습니다. 다시 선택하세요.";
        return;
    }

    malSelected = candidate;
    qDebug() << "Player" << (curP == P1 ? 1 : 2)
             << "말" << (pieceIndex + 1) << "선택됨";
    playMode = MOVE;
}

// IPC에서도 재사용할 수 있도록 분리된 이동 함수
void yutboard::moveSelectedPiece()
{
    if (playMode != MOVE) return;

    qDebug() << "→ moveMal() 호출";
    moveMal(malSelected);  // 기존 이동 로직 실행
}

