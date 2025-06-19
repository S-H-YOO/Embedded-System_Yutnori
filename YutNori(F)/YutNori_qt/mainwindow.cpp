#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "button_ipc.cpp"
#include "defvar.h"
#include <QDebug>
#include "button_reader.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    startButtonListener();
    startButtonReceiver();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButtonInput(char input) {
    qDebug() << "버튼 입력 받음:" << input;

    switch (input) {
        case 'Y':
            selectYutResult();
            break;
        case 'M':
            moveSelectedPiece();
            break;
        case '1':
            selectPiece(0);
            break;
        case '2':
            selectPiece(1);
            break;
        case 'U':
        case 'D':
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

void MainWindow::selectYutResult() {
    if (playMode == TOSS) {
        ready = ON;
        qDebug() << "버튼 입력으로 윷 결과 준비 상태 설정됨 (ready = ON)";
    } else {
        qDebug() << "현재 윷을 던질 수 없는 상태입니다. (playMode != TOSS)";
    }
}

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

