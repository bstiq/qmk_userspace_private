#ifndef SCREEN_POMODORO_H
#define SCREEN_POMODORO_H

lv_obj_t *init_screen_pomodoro(void) ;
void refresh_screen_pomodoro(void);
lv_obj_t *ui_create_pomodoro_title(lv_obj_t *cont);
lv_obj_t *ui_create_pomodoro_timer(lv_obj_t *cont);

#endif