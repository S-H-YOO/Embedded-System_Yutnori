#ifndef BUTTON_FND_H
#define BUTTON_FND_H

#define BUTTON_CODE_MOVE    115  // 예: KEY_VOLUMEUP
#define BUTTON_CODE_DELETE  158  // 예: KEY_BACK
#define BUTTON_CODE_EXTRA   114  // 예: KEY_VOLUMEDOWN (attempt++)

void move_cursor_left();
void delete_and_shift();
void run_button_handler_once(int fd);

#endif
