#include "lvgl.h"
#include <ctype.h>
#include QMK_KEYBOARD_H

void display_init(void);
void style_init_all(void);

void ui_init_layer_name(lv_obj_t *label);
void housekeeping_task_display(void);
void housekeeping_task_screen_base(void);
void housekeeping_task_screen_pointer(void);
void housekeeping_task_screen_rgb(void);
void housekeeping_task_screen_rgb(void);
void housekeeping_task_screen_layer_name(void);
void ui_init_button_mod_indicator(lv_obj_t *label);
lv_obj_t *ui_create_mod_button(lv_obj_t *cont, lv_obj_t **label_ptr, const char *text, bool force_new_track);

void event_screen_base_update_mods(lv_event_t *e);
void event_screen_pointer_sniping_toggle(lv_event_t *e);
void event_screen_pointer_scroll_toggle(lv_event_t *e);

void update_mod_button(uint8_t mods_active, uint8_t MASK, lv_obj_t *ui_button_mod);
void update_theme_based_on_layer(void);

bool process_records_display(uint16_t keycode, keyrecord_t *record);
void update_dilemma_status(void);

const char *rgb_matrix_get_effect_name(void);

static lv_obj_t *create_secondary_text(lv_obj_t *cont, const char *text, bool new_track, uint8_t flex);
static lv_obj_t *create_progress_bar(lv_obj_t *cont, uint8_t flex, uint8_t height);
static lv_obj_t *create_number_label(lv_obj_t *cont, uint8_t flex);