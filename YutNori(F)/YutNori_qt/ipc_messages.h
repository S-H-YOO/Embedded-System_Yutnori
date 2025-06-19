#ifndef IPC_MESSAGES_H
#define IPC_MESSAGES_H

#ifdef __cplusplus
extern "C" {
#endif

// 메시지 큐 키 생성을 위한 기준 경로 및 ID
#define MSG_KEY_PATH "/tmp"     // 반드시 실제 존재하는 경로여야 함
#define MSG_KEY_ID   1234       // msgget에서 사용하는 ID 값

// 수신 메시지 타입
#define MSG_TYPE_YUT 1          // 윷 결과 수신용 타입
#define MSG_TYPE_MOVE_MAL 2

// 메시지 구조체 정의
struct MyMsgBuf {
    long mtype;         // 메시지 타입 (예: MSG_TYPE_YUT)
    int yut_result;     // 윷 결과: -1(빽도), 1(도), 2(개), ..., 5(모), 8(빽도)
};

typedef struct {
    long mtype;
    int mal_id;       // 0: 말1, 1: 말2
    int move_steps;   // 이동 칸 수
} MoveMalMsgBuf;

#ifdef __cplusplus
}
#endif

#endif  // IPC_MESSAGES_H

