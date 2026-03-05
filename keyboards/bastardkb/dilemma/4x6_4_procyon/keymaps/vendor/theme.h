#include "lvgl.h"

void init_themes(void);
void apply_theme_btn_normal(lv_style_t *btn, ui_theme_mod_btn btn_theme);

typedef struct {
    lv_font_t font;
    uint8_t border_radius;
    uint16_t bg_opacity;
    lv_color_t bg_color;
    lv_color_t border_color;
    lv_color_t text_color;
    uint8_t border_width;
} ui_theme_mod_btn;

typedef struct {
    lv_font_t font;
    uint8_t border_radius;
    lv_color_t border_color;
    lv_color_t text_color;
    lv_color_t bg_color;
    uint8_t border_width;
} ui_theme_layer_name;

typedef struct {
    lv_font_t font;
} ui_theme_secondary_label;

typedef struct {
    uint8_t border_radius;
    lv_color_t bg_color;
} ui_theme_bar;

typedef struct {
    uint8_t border_radius;
    uint8_t border_width;
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