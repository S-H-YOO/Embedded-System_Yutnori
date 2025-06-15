#ifndef IPC_RECEIVER_H
#define IPC_RECEIVER_H

#include <QThread>
#include <QDebug>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "ipc_messages.h" // IPC 메시지 구조체 정의를 위해 필요

#include <QObject>

extern "C" {
#include "ipc_messages.h" // 백엔드 C 코드와 동일한 메시지 구조체 정의 사용
}

class IpcReceiver : public QObject
{
    Q_OBJECT

public:
    explicit IpcReceiver(QObject *parent = nullptr);
    ~IpcReceiver();

signals:
    // 윷 결과가 수신되었을 때 Qt 메인 스레드에 보낼 시그널
    void yutResultReceived(int result);
    void moveMalRequested(int malIdx, int steps);

private slots:
    void readMessageQueue();

protected:

private:
    int msg_queue_id;
    int msgQueueId;
};

#endif // IPC_RECEIVER_H
