#ifndef INPUT_KEYMAP_H
#define INPUT_KEYMAP_H

#include <linux/input.h>
#include <unordered_map>

// ev.code → 의미 있는 문자로 매핑
static const std::unordered_map<int, char> keycodeToChar = {
    {KEY_HOME,       'Y'},   // 윷 결과 선택
    {KEY_BACK,       'M'},   // 말 이동
    {KEY_SEARCH,     '1'},   // 현재 플레이어 1번 말 선택
    {KEY_MENU,       '2'},   // 현재 플레이어 2번 말 선택
    {KEY_VOLUMEUP,   'U'},   // (미정)
    {KEY_VOLUMEDOWN, 'D'}    // (미정)
};

#endif // INPUT_KEYMAP_H

