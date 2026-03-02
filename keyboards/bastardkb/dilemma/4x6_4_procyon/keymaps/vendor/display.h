#include "lvgl.h"
#include <ctype.h>
#include QMK_KEYBOARD_H

void display_init(void);
void style_init_mod_button(void);
void style_pressed_init_mod_indicator(void);
void style_bar_init(void);
void style_line_init(void);
void style_flex_container_init(void);
void style_layer_name_init(void);
void style_secondary_labels_init(void);

void ui_init_layer_name(lv_obj_t *label);
void housekeeping_task_display(void);
void housekeeping_task_screen_base(void);
void housekeeping_task_screen_pointer(void);
void housekeeping_task_screen_rgb(void);
void housekeeping_task_screen_rgb(void);
void housekeeping_task_screen_layer_name(void);
void ui_init_button_mod_indicator(lv_obj_t *label);

void event_screen_base_update_mods(lv_event_t *e);
void event_screen_pointer_sniping_toggle(lv_event_t *e);
void event_screen_pointer_scroll_toggle(lv_event_t *e);

void update_mod_button(uint8_t mods_active, uint8_t MASK, lv_obj_t *ui_button_mod);

bool process_records_display(uint16_t keycode, keyrecord_t *record);
void update_dilemma_status(void);

const char *rgb_matrix_get_effect_name(void);