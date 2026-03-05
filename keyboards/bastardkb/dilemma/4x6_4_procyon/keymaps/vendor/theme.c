#include "theme.h"

LV_FONT_DECLARE(montserratbold14);
LV_FONT_DECLARE(montserratbold13);

ui_theme default_theme;

void init_themes(void) {
    ui_theme default_theme = {
        .btn_normal = {
            .font = &montserratbold14,
            .radius = 6,
            .bg_opa = LV_OPA_COVER,
            .bg_color = lv_color_make(23, 26, 31),
            .border_color = lv_color_make(50, 55, 67),
            .text_color = lv_color_white(),
            2,
        },
        .btn_pressed = {
            .font = &montserratbold14,
            .radius = 6,
            .bg_opa = LV_OPA_COVER,
            .bg_color = lv_color_make(71, 133, 239),
            .border_color = lv_color_white(),
            .text_color = lv_color_white(),
            .border_width = 2,
        },
        .layer_name = {
            .font = &montserratbold14,
            .radius = 6,
            .bg_color = lv_color_black(),
            .border_color = lv_color_make(50, 55, 67),
            .text_color = lv_color_white(),
            .border_width = 2,
        },
        .secondary_labels = {
            .font = &montserratbold13,
        },
        .line = {
            .radius = 0,
            .bg_color = lv_color_make(71, 133, 239),
        },
        .line_background = {
            .radius = 3,
            .border_width = 0,
            .bg_color = lv_color_make(50, 55, 67),
        },
    };
}

void apply_theme_btn_normal(lv_style_t *btn, ui_theme theme) {
    lv_style_set_text_font(btn, theme.btn_normal.font);
    lv_style_set_radius(btn, theme.btn_normal.border_radius);
    lv_style_set_bg_opa(btn, theme.btn_normal.bg_opa);
    lv_style_set_bg_color(btn, theme.btn_normal.bg_color);
    lv_style_set_border_color(btn, theme.btn_normal.border_color);
    lv_style_set_border_width(btn, theme.btn_normal.border_width);
    lv_style_set_text_color(btn, theme.btn_normal.text_color);
}