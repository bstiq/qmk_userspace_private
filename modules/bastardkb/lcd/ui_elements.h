#include "lvgl.h"

/* mod button pairs: GUI, ALT, CTRL, SHIFT */
typedef struct {
    lv_obj_t *button;
    lv_obj_t *label;
    uint8_t   mod_mask;
} mod_button_pair_t;

// TODO once removed ui_styles from lcd.c, move this into ui_elements.c to make it private to ui_elements 
typedef struct {
    lv_style_t mod_btn;
    lv_style_t bar;
    lv_style_t bar_background;
    lv_style_t mod_btn_pressed;
    lv_style_t flex_container;
    lv_style_t line;
    lv_style_t layer_name;
    lv_style_t line_background;
    lv_style_t secondary_labels;
} ui_styles_t;

ui_styles_t ui_styles;

void              ui_init_layer_name(lv_obj_t *label);
void              ui_init_button_mod_indicator(lv_obj_t *label);
mod_button_pair_t ui_create_mod_button(lv_obj_t *cont, const char *text, bool force_new_track, uint8_t mod_mask);
lv_obj_t         *ui_create_secondary_text(lv_obj_t *cont, const char *text, bool new_track, uint8_t flex);
lv_obj_t         *ui_create_progress_bar(lv_obj_t *cont, uint8_t flex);
lv_obj_t         *ui_create_number_label(lv_obj_t *cont, uint8_t flex);
lv_obj_t         *ui_create_line_separator(lv_obj_t *cont, uint8_t flex, uint8_t height);