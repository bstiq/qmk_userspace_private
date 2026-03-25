#ifndef MENU_H
#define MENU_H

#include "lvgl.h"
#include "utilities.h"
#include QMK_KEYBOARD_H

typedef struct{
    lv_obj_t *obj;
    void (*update_function)(void);
} obj_update_dilemma_menu_t;

bool process_record_menu(uint16_t keycode, keyrecord_t *record, const obj_update_dilemma_menu_t menus[], uint8_t *menu_index, const uint8_t menu_size);
void release_all_buttons(const obj_update_dilemma_menu_t menus[], const uint8_t menu_size);
void press_menu_button(const obj_update_dilemma_menu_t menu);
void release_menu_button(const obj_update_dilemma_menu_t menu);
lv_obj_t *ui_create_menu_line(lv_obj_t *cont, const char *text);

#endif