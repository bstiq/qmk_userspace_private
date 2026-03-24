#ifndef SCREEN_POMODORO_H
#define SCREEN_POMODORO_H

void init_screen_pomodoro(void) ;
void refresh_screen_pomodoro(void);
lv_obj_t *ui_create_pomodoro_title(lv_obj_t *cont);
lv_obj_t *ui_create_pomodoro_time(lv_obj_t *cont);
lv_obj_t *ui_create_pomodoro_arc(lv_obj_t *cont);

lv_obj_t *ui_create_pomodoro_arc(lv_obj_t *cont);
void update_pomodoro_arc(lv_obj_t *obj);
void update_pomodoro_time(lv_obj_t *obj);
bool process_record_screen_pomodoro(uint16_t keycode, keyrecord_t *record);

void menu_pomodoro_back_to_main(lv_obj_t *obj);
void load_module_screen_pomodoro(void);

#endif