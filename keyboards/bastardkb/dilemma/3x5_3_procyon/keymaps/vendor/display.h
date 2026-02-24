#include "lvgl.h"
#include <ctype.h>
#include QMK_KEYBOARD_H

// screen stuff
extern lv_obj_t *ui_screen_base;
extern lv_obj_t *ui_screen_pointer;

extern lv_obj_t *ui_label_layer_name_base;
// extern lv_obj_t *ui_label_layer_name_pointer;

// base layer
extern lv_obj_t *ui_label_mod_gui;
extern lv_obj_t *ui_button_mod_gui;
extern lv_obj_t *ui_label_mod_shift;
extern lv_obj_t *ui_button_mod_shift;
extern lv_obj_t *ui_label_mod_control;
extern lv_obj_t *ui_button_mod_control;
extern lv_obj_t *ui_label_mod_alt;
extern lv_obj_t *ui_button_mod_alt;

// pointer layer
extern lv_obj_t *ui_label_dpi;
extern lv_obj_t *ui_bar_dpi;
extern lv_obj_t *ui_label_s_dpi;
extern lv_obj_t *ui_bar_s_dpi;
extern lv_obj_t *ui_label_sniping;
extern lv_obj_t *ui_button_sniping;
extern lv_obj_t *ui_label_scroll;
extern lv_obj_t *ui_button_scroll;

// rgb info
extern lv_obj_t *ui_label_rgb;
extern lv_obj_t *ui_bar_rgb;
extern lv_obj_t *ui_label_rgb_effect;

// styles
extern lv_style_t style_btn;
extern lv_style_t style_bar;
extern lv_style_t style_bar_background;
extern lv_style_t style_btn_pressed;
extern lv_style_t style_flex_container;

extern uint8_t    last_mods;

void display_init(void);
void style_init_mod_indicator(void);
void style_pressed_init_mod_indicator(void);
void style_bar_init(void);
void style_flex_container_init(void);

// void ui_layer_change(lv_event_t *e);
void ui_init_layer_name(lv_obj_t *label, const char *layer_name);
void housekeeping_task_display(void);
void housekeeping_task_screen_base(void);
void housekeeping_task_screen_pointer(void);
void housekeeping_task_screen_rgb(void);
void housekeeping_task_screen_rgb(void);
void ui_init_button_mod_indicator(lv_obj_t *label, int x, int y);

void event_screen_base_update_mods(lv_event_t *e);
void event_screen_pointer_sniping_toggle(lv_event_t *e);
void event_screen_pointer_scroll_toggle(lv_event_t *e);

void update_mod_button(uint8_t mods_active, uint8_t MASK, lv_obj_t *ui_button_mod);

bool process_records_display(uint16_t keycode, keyrecord_t *record);

static uint8_t prev_layer;