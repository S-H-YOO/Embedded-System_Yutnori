#ifndef IPC_MESSAGES_H
#define IPC_MESSAGES_H

// 메시지 큐 키를 생성하기 위한 파일 경로 및 ID
#define MSG_KEY_PATH "/tmp"
#define MSG_KEY_ID   1234

// 메시지 타입
#define MSG_TYPE_YUT 1
#define MSG_TYPE_MOVE_MAL 2

// 메시지 구조체 정의
typedef struct {
    long mtype;        // 메시지 타입 (예: MSG_TYPE_YUT)
    int yut_result;    // 전송할 윷 결과 (1~5, 8)
} MyMsgBuf;

typedef struct {
    long mtype;      // 메시지 타입 (예: MSG_TYPE_MOVE_MAL)
    int mal_id;      // 말의 고유 ID (P11, P12, P21, P22 중 하나)
    int move_steps;  // 이동할 칸 수
} MoveMalMsgBuf;

#endif
