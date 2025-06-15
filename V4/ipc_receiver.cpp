#include "ipc_receiver.h"
#include <QTimer>
#include <QDebug>
#include <cstring>     // strerror
#include <errno.h>     // errno
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ipc_messages.h"


// 생성자
IpcReceiver::IpcReceiver(QObject *parent) : QObject(parent), msg_queue_id(-1)
{
    // 메시지 큐 키 생성
    key_t msg_queue_key = ftok(MSG_KEY_PATH, MSG_KEY_ID);
    if (msg_queue_key == -1) {
        qDebug() << "ftok failed in IPC_Receiver:" << strerror(errno);
        msg_queue_id = -1;
        return;
    }

    // 기존 메시지 큐 ID 가져오기
    msg_queue_id = msgget(msg_queue_key, 0);
    if (msg_queue_id == -1) {
        qDebug() << "msgget failed in IPC_Receiver:" << strerror(errno);
        msg_queue_id = -1;
        return;
    }

    qDebug() << "IPC_Receiver connected to message queue with ID:" << msg_queue_id;

    // 타이머 설정
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &IpcReceiver::readMessageQueue);
    timer->start(100);  // 100ms마다 메시지 큐 확인
}

// 메시지 큐에서 수신 처리
void IpcReceiver::readMessageQueue()
{
    if (msg_queue_id == -1)
        return;

    // [1] 윷 결과 수신 (mtype = MSG_TYPE_YUT)
    struct MyMsgBuf yut_msg;
    ssize_t ret = msgrcv(msg_queue_id, &yut_msg, sizeof(yut_msg.yut_result), MSG_TYPE_YUT, IPC_NOWAIT);
    if (ret > 0) {
        emit yutResultReceived(yut_msg.yut_result);
        qDebug() << "▶ 수신된 윷 결과:" << yut_msg.yut_result;
    }

    // [2] 말 이동 요청 수신 (mtype = MSG_TYPE_MOVE_MAL)
    struct MoveMalMsgBuf mal_msg;
    ret = msgrcv(msg_queue_id, &mal_msg, sizeof(mal_msg) - sizeof(long), MSG_TYPE_MOVE_MAL, IPC_NOWAIT);
    if (ret > 0) {
        emit moveMalRequested(mal_msg.selected_mal_idx, mal_msg.steps_to_move);
        qDebug() << "▶ 수신된 말 이동 요청 → 말:" << mal_msg.selected_mal_idx
                 << ", 칸 수:" << mal_msg.steps_to_move;
    }
}

IpcReceiver::~IpcReceiver() {
    // 필요 시 자원 정리 코드 삽입 가능
    qDebug() << "IpcReceiver 소멸자 호출됨.";
}
