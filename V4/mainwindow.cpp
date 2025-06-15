#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "button_ipc.cpp"
#include "defvar.h"  // result, playMode 등 외부 전역변수 정의
#include <QDebug>
#include "button_reader.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startButtonListener();    // FIFO 생성 및 버튼 입력 감지 시작
    startButtonReceiver();    // Qt 쪽 FIFO 읽기 시작

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButtonInput(char input) {
    qDebug() << "버튼 입력 받음:" << input;

    // 상태 텍스트 갱신 (label_status가 존재한다면)
//    if (ui && ui->label_status)
//        ui->label_status->setText(QString("입력: %1").arg(input));

    switch (input) {
        case 'Y':  // KEY_HOME: 윷 결과 선택
            selectYutResult();  // 윷 결과 중 선택 (ex: 도, 개, 걸 등 선택)
            break;

        case 'M':  // KEY_BACK: 윷 결과만큼 말 이동
            moveSelectedPiece();  // 선택된 말을 윷 결과만큼 이동
            break;

        case '1':  // KEY_SEARCH: 현재 플레이어의 1번 말 선택
            selectPiece(0);  // 첫 번째 말 선택
            break;

        case '2':  // KEY_MENU: 현재 플레이어의 2번 말 선택
            selectPiece(1);  // 두 번째 말 선택
            break;

        case 'U':  // KEY_VOLUMEUP: 추후 기능 추가 예정
        case 'D':  // KEY_VOLUMEDOWN: 추후 기능 추가 예정
            qDebug() << "아직 정의되지 않은 입력입니다.";
            break;

        default:
            qDebug() << "알 수 없는 버튼 입력:" << input;
            break;
    }
}


void MainWindow::selectPiece(int pieceIndex) {
    int candidate = (curP == P1 ? P11 : P21) + pieceIndex;
    if (status[candidate] == SCORE) {
        qDebug() << "말이 이미 도착했습니다. 다시 선택하세요.";
        return;
    }
    malSelected = candidate;
    playMode = MOVE;
    qDebug() << "플레이어" << (curP == P1 ? 1 : 2) << "의 말" << (pieceIndex + 1) << "선택됨";
}


//void MainWindow::selectPiece(int index) {
//    qDebug() << "selectPiece() 호출됨. index =" << index;
//
//    // 예: index == 0 → 첫 번째 말, index == 1 → 두 번째 말
//    // 선택된 말을 내부 상태로 저장
//    selectedPieceIndex = index;
//
//    // UI 상태 표시
//    if (ui) {
//        QString msg = QString("플레이어의 %1번 말 선택됨").arg(index + 1);
//        ui->label_status->setText(msg);
//    }
//
//    // LED로도 상태 표시하고 싶다면 여기에 추가
//}
//
// 📄 mainwindow.cpp
void MainWindow::selectYutResult() {
    if (playMode == TOSS) {
        ready = ON;
        qDebug() << "버튼 입력으로 윷 결과 준비 상태 설정됨 (ready = ON)";
    } else {
        qDebug() << "현재 윷을 던질 수 없는 상태입니다. (playMode != TOSS)";
    }
}


//void MainWindow::selectYutResult() {
//    if (result < -1 || result > 5) {
//        qDebug() << "유효하지 않은 윷 결과입니다: " << result;
//        return;
//    }
//
//    playMode = SELECT;  // 다음 단계로 넘어감
//    qDebug() << "선택된 윷 결과로 SELECT 상태 진입:" << result;
//}
// mainwindow.cpp
void MainWindow::startButtonReceiver() {
    pthread_t tid;
    pthread_create(&tid, NULL, readButtonFIFO, this);
    pthread_detach(tid);
}

void MainWindow::moveSelectedPiece() {
    if (ui && ui->widget) {
        ui->widget->moveSelectedPiece();
    }
}

