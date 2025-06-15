#ifndef IPC_MESSAGES_H
#define IPC_MESSAGES_H

// C/C++ 모두에서 사용 가능하도록 C 스타일로 정의
#ifdef __cplusplus
extern "C" {
#endif

#include <sys/ipc.h> // key_t 타입을 위해 필요

// 메시지 구조체 정의 (C/C++ 호환)
// 반드시 long mtype으로 시작해야 합니다.
struct MyMsgBuf {
    long mtype;       // 메시지 타입 (필수, 0보다 커야 함)
    int yut_result;   // WiFi로 수신한 윷 결과 값
};

// 말 이동을 위한 메시지 구조체
struct MoveMalMsgBuf {
    long mtype;           // 메시지 타입 (필수, 0보다 커야 함)
    int selected_mal_idx; // 선택된 말의 인덱스 (Player1의 말1: 0, Player1의 말2: 1, Player2의 말1: 2, Player2의 말2: 3 등)
    int steps_to_move;    // 이동할 칸 수
};


// 메시지 큐 키 정의 (송신자와 동일해야 함)
#define MSG_KEY_PATH "/tmp" // ftok가 사용할 경로 (실제 파일은 아님)
#define MSG_KEY_ID 'A'      // ftok가 사용할 ID (0이 아닌 정수)
#define MSG_TYPE_YUT 1      // 윷 결과를 위한 메시지 타입
#define MSG_TYPE_MOVE_MAL 2

#ifdef __cplusplus
}
#endif

#endif // IPC_MESSAGES_H
