#include "lvgl.h"

typedef struct {
    const lv_font_t *font;
    const uint8_t border_radius;
    const uint16_t bg_opacity;
    const lv_color_t bg_color;
    const lv_color_t border_color;
    const lv_color_t text_color;
    uint8_t border_width;
} ui_theme_mod_btn;

typedef struct {
    const lv_font_t *font;
    const uint8_t border_radius;
    const lv_color_t border_color;
    const lv_color_t text_color;
    const lv_color_t bg_color;
    uint8_t border_width;
} ui_theme_layer_name;

typedef struct {
    const lv_font_t *font;
} ui_theme_secondary_label;

typedef struct {
    const uint8_t border_radius;
    const lv_color_t bg_color;
} ui_theme_bar;

typedef struct {
    const uint8_t border_radius;
    const uint8_t border_width;
} ui_theme_bar_background;

typedef struct {
    ui_theme_mod_btn btn_normal;
    ui_theme_mod_btn btn_pressed;
    ui_theme_layer_name layer_name;
    ui_theme_secondary_label secondary_labels;
    ui_theme_bar bar;
    ui_theme_bar_background bar_background;
    // SEPARATOR LINE -- TODO
} ui_theme;